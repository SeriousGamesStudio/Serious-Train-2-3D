/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2014 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#include "OgreGLES2Prerequisites.h"
#include "OgreGpuProgram.h"
#include "OgreHighLevelGpuProgramManager.h"
#include "OgreLogManager.h"
#include "OgreRoot.h"
#include "OgreStringConverter.h"
#include "OgreGLUtil.h"
#include "OgreGLES2RenderSystem.h"
#include "OgreGLES2Support.h"

#include "OgreGLSLESProgram.h"
#include "OgreGLSLESProgramManager.h"
#include "OgreGLSLPreprocessor.h"

namespace Ogre {
    //-----------------------------------------------------------------------
#if !OGRE_NO_GLES2_GLSL_OPTIMISER
    GLSLESProgram::CmdOptimisation GLSLESProgram::msCmdOptimisation;
#endif
    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    GLSLESProgram::GLSLESProgram(ResourceManager* creator, 
        const String& name, ResourceHandle handle,
        const String& group, bool isManual, ManualResourceLoader* loader)
        : GLSLShaderCommon(creator, name, handle, group, isManual, loader)
        , mGLShaderHandle(0)
        , mGLProgramHandle(0)
#if !OGRE_NO_GLES2_GLSL_OPTIMISER
        , mIsOptimised(false)
        , mOptimiserEnabled(false)
#endif
    {
        if (createParamDictionary("GLSLESProgram"))
        {
            setupBaseParamDictionary();
            ParamDictionary* dict = getParamDictionary();

            dict->addParameter(ParameterDef("preprocessor_defines", 
                                            "Preprocessor defines use to compile the program.",
                                            PT_STRING),&msCmdPreprocessorDefines);
#if !OGRE_NO_GLES2_GLSL_OPTIMISER
            dict->addParameter(ParameterDef("use_optimiser", 
                                            "Should the GLSL optimiser be used. Default is false.",
                                            PT_BOOL),&msCmdOptimisation);
#endif
        }
        // Manually assign language now since we use it immediately
        mSyntaxCode = "glsles";

        // There is nothing to load
        mLoadFromFile = false;
    }
    //---------------------------------------------------------------------------
    GLSLESProgram::~GLSLESProgram()
    {
        // Have to call this here reather than in Resource destructor
        // since calling virtual methods in base destructors causes crash
        if (isLoaded())
        {
            unload();
        }
        else
        {
            unloadHighLevel();
        }
    }
    //---------------------------------------------------------------------------
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID || OGRE_PLATFORM == OGRE_PLATFORM_EMSCRIPTEN
    void GLSLESProgram::notifyOnContextLost()
    {
        unloadHighLevelImpl();
    }
#endif
    GLuint GLSLESProgram::createGLProgramHandle() {
        if(!Root::getSingleton().getRenderSystem()->getCapabilities()->hasCapability(RSC_SEPARATE_SHADER_OBJECTS))
            return 0;

        if (mGLProgramHandle)
            return mGLProgramHandle;

        OGRE_CHECK_GL_ERROR(mGLProgramHandle = glCreateProgram());
#if OGRE_PLATFORM != OGRE_PLATFORM_NACL
        if(Root::getSingleton().getRenderSystem()->getCapabilities()->hasCapability(RSC_DEBUG))
        {
            glLabelObjectEXT(GL_PROGRAM_OBJECT_EXT, mGLProgramHandle, 0, mName.c_str());
        }
#endif

        return mGLProgramHandle;
    }

    bool GLSLESProgram::compile(bool checkErrors)
    {
        if (mCompiled == 1)
        {
            return true;
        }
        // Only create a shader object if glsl es is supported
        if (isSupported())
        {
            // Create shader object
            GLenum shaderType = 0x0000;
            if (mType == GPT_VERTEX_PROGRAM)
            {
                shaderType = GL_VERTEX_SHADER;
            }
            else if (mType == GPT_FRAGMENT_PROGRAM)
            {
                shaderType = GL_FRAGMENT_SHADER;
            }
            OGRE_CHECK_GL_ERROR(mGLShaderHandle = glCreateShader(shaderType));

#if OGRE_PLATFORM != OGRE_PLATFORM_NACL
            if(Root::getSingleton().getRenderSystem()->getCapabilities()->hasCapability(RSC_DEBUG))
            {
                glLabelObjectEXT(GL_SHADER_OBJECT_EXT, mGLShaderHandle, 0, mName.c_str());
            }
#endif
            createGLProgramHandle();
        }

        // Add preprocessor extras and main source
        if (!mSource.empty())
        {
            const RenderSystemCapabilities* caps = Root::getSingleton().getRenderSystem()->getCapabilities();
            // Fix up the source in case someone forgot to redeclare gl_Position
            if (caps->hasCapability(RSC_GLSL_SSO_REDECLARE) && mType == GPT_VERTEX_PROGRAM)
            {
                size_t versionPos = mSource.find("#version");
                int shaderVersion = StringConverter::parseInt(mSource.substr(versionPos+9, 3));
                size_t belowVersionPos = mSource.find("\n", versionPos) + 1;

                if(shaderVersion >= 300) {
                    // Check that it's missing and that this shader has a main function, ie. not a child shader.
                    if(mSource.find("out highp vec4 gl_Position") == String::npos)
                    {
                        mSource.insert(belowVersionPos, "out highp vec4 gl_Position;\nout highp float gl_PointSize;\n");
                    }
                    if(mSource.find("#extension GL_EXT_separate_shader_objects : require") == String::npos)
                    {
                        mSource.insert(belowVersionPos, "#extension GL_EXT_separate_shader_objects : require\n");
                    }
                }
            }
    
#if !OGRE_NO_GLES2_GLSL_OPTIMISER
            const char *source = (getOptimiserEnabled() && getIsOptimised()) ? mOptimisedSource.c_str() : mSource.c_str();
#else
            const char *source = mSource.c_str();
#endif

            OGRE_CHECK_GL_ERROR(glShaderSource(mGLShaderHandle, 1, &source, NULL));
        }

        if (checkErrors)
            GLSLES::logObjectInfo("GLSL ES compiling: " + mName, mGLShaderHandle);

        OGRE_CHECK_GL_ERROR(glCompileShader(mGLShaderHandle));

        // Check for compile errors
        OGRE_CHECK_GL_ERROR(glGetShaderiv(mGLShaderHandle, GL_COMPILE_STATUS, &mCompiled));
        if(!mCompiled && checkErrors)
        {
            String message = GLSLES::logObjectInfo("GLSL ES compile log: " + mName, mGLShaderHandle);
            checkAndFixInvalidDefaultPrecisionError(message);
        }

        // Log a message that the shader compiled successfully.
        if (mCompiled && checkErrors)
            GLSLES::logObjectInfo("GLSL ES compiled: " + mName, mGLShaderHandle);

        return (mCompiled == 1);
    }

#if !OGRE_NO_GLES2_GLSL_OPTIMISER   
    //-----------------------------------------------------------------------
    void GLSLESProgram::setOptimiserEnabled(bool enabled) 
    { 
        if(mOptimiserEnabled != enabled && mOptimiserEnabled && mCompiled == 1)
        {
            OGRE_CHECK_GL_ERROR(glDeleteShader(mGLShaderHandle));

            if(Root::getSingleton().getRenderSystem()->getCapabilities()->hasCapability(RSC_SEPARATE_SHADER_OBJECTS))
            {
                OGRE_CHECK_GL_ERROR(glDeleteProgram(mGLProgramHandle));
            }
            
            mGLShaderHandle = 0;
            mGLProgramHandle = 0;
            mCompiled = 0;
        }
        mOptimiserEnabled = enabled; 
    }
#endif
    //-----------------------------------------------------------------------
    void GLSLESProgram::createLowLevelImpl(void)
    {
    }
    //-----------------------------------------------------------------------
    void GLSLESProgram::unloadHighLevelImpl(void)
    {
        if (isSupported())
        {
//            LogManager::getSingleton().logMessage("Deleting shader " + StringConverter::toString(mGLShaderHandle) +
//                                                  " and program " + StringConverter::toString(mGLProgramHandle));
            OGRE_CHECK_GL_ERROR(glDeleteShader(mGLShaderHandle));

            if(Root::getSingleton().getRenderSystem()->getCapabilities()->hasCapability(RSC_SEPARATE_SHADER_OBJECTS))
            {
                OGRE_CHECK_GL_ERROR(glDeleteProgram(mGLProgramHandle));
            }
            // destroy all programs using this shader
            GLSLESProgramManager::getSingletonPtr()->destroyAllByShader(this);

            
            mGLShaderHandle = 0;
            mGLProgramHandle = 0;
            mCompiled = 0;
            mLinked = 0;
        }
    }
    //-----------------------------------------------------------------------
    void GLSLESProgram::buildConstantDefinitions() const
    {
        // We need an accurate list of all the uniforms in the shader, but we
        // can't get at them until we link all the shaders into a program object.

        // Therefore instead, parse the source code manually and extract the uniforms
        createParameterMappingStructures(true);
        GLSLESProgramManager::getSingleton().extractUniformsFromGLSL(mSource, *mConstantDefs, mName);
    }

    //-----------------------------------------------------------------------
#if !OGRE_NO_GLES2_GLSL_OPTIMISER
    String GLSLESProgram::CmdOptimisation::doGet(const void *target) const
    {
        return StringConverter::toString(static_cast<const GLSLESProgram*>(target)->getOptimiserEnabled());
    }
    void GLSLESProgram::CmdOptimisation::doSet(void *target, const String& val)
    {
        static_cast<GLSLESProgram*>(target)->setOptimiserEnabled(StringConverter::parseBool(val));
    }
#endif
    //-----------------------------------------------------------------------
    void GLSLESProgram::attachToProgramObject( const GLuint programObject )
    {
//        LogManager::getSingleton().logMessage("Attaching shader " + StringConverter::toString(mGLShaderHandle) +
//                                              " to program " + StringConverter::toString(programObject));
        OGRE_CHECK_GL_ERROR(glAttachShader(programObject, mGLShaderHandle));
    }
    //-----------------------------------------------------------------------
    void GLSLESProgram::detachFromProgramObject( const GLuint programObject )
    {
//        LogManager::getSingleton().logMessage("Detaching shader " + StringConverter::toString(mGLShaderHandle) +
//                                              " to program " + StringConverter::toString(programObject));
        OGRE_CHECK_GL_ERROR(glDetachShader(programObject, mGLShaderHandle));
    }

    //-----------------------------------------------------------------------
    const String& GLSLESProgram::getLanguage(void) const
    {
        static const String language = "glsles";

        return language;
    }
    //-----------------------------------------------------------------------
    Ogre::GpuProgramParametersSharedPtr GLSLESProgram::createParameters( void )
    {
        GpuProgramParametersSharedPtr params = HighLevelGpuProgram::createParameters();
        params->setTransposeMatrices(true);
        return params;
    }
    //-----------------------------------------------------------------------
    void GLSLESProgram::checkAndFixInvalidDefaultPrecisionError( String &message )
    {
        String precisionQualifierErrorString = ": 'Default Precision Qualifier' : invalid type Type for default precision qualifier can be only float or int";
        vector< String >::type linesOfSource = StringUtil::split(mSource, "\n");
        if( message.find(precisionQualifierErrorString) != String::npos )
        {
            LogManager::getSingleton().logMessage("Fixing invalid type Type for default precision qualifier by deleting bad lines the re-compiling");

            // remove relevant lines from source
            vector< String >::type errors = StringUtil::split(message, "\n");

            // going from the end so when we delete a line the numbers of the lines before will not change
            for(int i = static_cast<int>(errors.size()) - 1 ; i != -1 ; i--)
            {
                String & curError = errors[i];
                size_t foundPos = curError.find(precisionQualifierErrorString);
                if(foundPos != String::npos)
                {
                    String lineNumber = curError.substr(0, foundPos);
                    size_t posOfStartOfNumber = lineNumber.find_last_of(':');
                    if (posOfStartOfNumber != String::npos)
                    {
                        lineNumber = lineNumber.substr(posOfStartOfNumber + 1, lineNumber.size() - (posOfStartOfNumber + 1));
                        if (StringConverter::isNumber(lineNumber))
                        {
                            int iLineNumber = StringConverter::parseInt(lineNumber);
                            linesOfSource.erase(linesOfSource.begin() + iLineNumber - 1);
                        }
                    }
                }
            }   
            // rebuild source
            StringStream newSource; 
            for(size_t i = 0; i < linesOfSource.size()  ; i++)
            {
                newSource << linesOfSource[i] << "\n";
            }
            mSource = newSource.str();

            const char *source = mSource.c_str();
            OGRE_CHECK_GL_ERROR(glShaderSource(mGLShaderHandle, 1, &source, NULL));

            if (compile())
            {
                LogManager::getSingleton().logMessage("The removing of the lines fixed the invalid type Type for default precision qualifier error.");
            }
            else
            {
                LogManager::getSingleton().logMessage("The removing of the lines didn't help.");
            }
        }
    }

    //-----------------------------------------------------------------------------
    void GLSLESProgram::bindProgram(void)
    {
        // Tell the Link Program Manager what shader is to become active
        switch (mType)
        {
            case GPT_VERTEX_PROGRAM:
                GLSLESProgramManager::getSingleton().setActiveVertexShader( this );
                break;
            case GPT_FRAGMENT_PROGRAM:
                GLSLESProgramManager::getSingleton().setActiveFragmentShader( this );
                break;
            case GPT_GEOMETRY_PROGRAM:
            default:
                break;
        }
    }

    //-----------------------------------------------------------------------------
    void GLSLESProgram::unbindProgram(void)
    {
        // Tell the Link Program Manager what shader is to become inactive
        if (mType == GPT_VERTEX_PROGRAM)
        {
            GLSLESProgramManager::getSingleton().setActiveVertexShader( NULL );
        }
        else if (mType == GPT_FRAGMENT_PROGRAM)
        {
            GLSLESProgramManager::getSingleton().setActiveFragmentShader( NULL );
        }
    }

    //-----------------------------------------------------------------------------
    void GLSLESProgram::bindProgramParameters(GpuProgramParametersSharedPtr params, uint16 mask)
    {
        // Link can throw exceptions, ignore them at this point
        try
        {
            // Activate the link program object
            GLSLESProgramCommon* linkProgram = GLSLESProgramManager::getSingleton().getActiveProgram();
            // Pass on parameters from params to program object uniforms
            linkProgram->updateUniforms(params, mask, mType);

        }
        catch (Exception& e) {}
    }

    //-----------------------------------------------------------------------------
    void GLSLESProgram::bindProgramSharedParameters(GpuProgramParametersSharedPtr params, uint16 mask)
    {
        // Link can throw exceptions, ignore them at this point
        try
        {
            // Activate the link program object
            GLSLESProgramCommon* linkProgram = GLSLESProgramManager::getSingleton().getActiveProgram();
            // Pass on parameters from params to program object uniforms
            linkProgram->updateUniformBlocks(params, mask, mType);
        }
        catch (Exception& e) {}
    }

    //-----------------------------------------------------------------------------
    void GLSLESProgram::bindProgramPassIterationParameters(GpuProgramParametersSharedPtr params)
    {
        // Activate the link program object
        GLSLESProgramCommon* linkProgram = GLSLESProgramManager::getSingleton().getActiveProgram();
        // Pass on parameters from params to program object uniforms
        linkProgram->updatePassIterationUniforms( params );
    }

    //-----------------------------------------------------------------------------
    size_t GLSLESProgram::calculateSize(void) const
    {
        size_t memSize = 0;

        // Delegate Names
        memSize += sizeof(GLuint);
        memSize += sizeof(GLenum);
        memSize += GpuProgram::calculateSize();

        return memSize;
    }
}
