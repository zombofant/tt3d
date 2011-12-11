/**********************************************************************
File name: Framebuffer.hpp
This file is part of: tt3d — Freeform transport simulation

LICENSE

The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations under
the License.

Alternatively, the contents of this file may be used under the terms of
the GNU General Public license (the  "GPL License"), in which case  the
provisions of GPL License are applicable instead of those above.

FEEDBACK & QUESTIONS

For feedback and questions about tt3d please e-mail one of the authors
named in the AUTHORS file.
**********************************************************************/
#ifndef _TT3D_GL_FRAMEBUFFER_H
#define _TT3D_GL_FRAMEBUFFER_H

#include <GL/glew.h>
#include "Base.hpp"
#include "Viewport.hpp"
#include "Pixelbuffer.hpp"

namespace tt3d {
namespace GL {
    
class FramebufferError: public Error {
    public:
        FramebufferError(std::string message):
            Error::Error(message)
        {};
        
        ~FramebufferError() throw() {};
};

class Framebuffer;

class Attachment {
    public:
        Attachment(const PixelbufferHandle pixelbuffer);
    protected:
        PixelbufferHandle _pixelbuffer;
        const glAttachFunc _attachFunc;
        const GLenum _attachKind;
    protected:
        void attach(const GLenum target);
        static bool isDepthFormat(const GLenum format);
        static bool isStencilFormat(const GLenum format);
    public:
        GLenum getFormat() const { return _pixelbuffer->getFormat(); };
        GLsizei getHeight() const { return _pixelbuffer->getHeight(); };
        GLsizei getWidth() const { return _pixelbuffer->getWidth(); };
    friend class Framebuffer;
};

class DepthAttachment: public Attachment {
    public:
        DepthAttachment(const PixelbufferHandle pixelbuffer):
            Attachment::Attachment(pixelbuffer) {};
    public:
        static bool isCompatible(const PixelbufferHandle pixelbuffer);
};
typedef boost::shared_ptr<DepthAttachment> DepthAttachmentHandle;

class ColourAttachment: public Attachment {
    public:
        ColourAttachment(const PixelbufferHandle pixelbuffer):
            Attachment::Attachment(pixelbuffer) {};
    public:
        static bool isCompatible(const PixelbufferHandle pixelbuffer);
};
typedef boost::shared_ptr<ColourAttachment> ColourAttachmentHandle;

class Framebuffer: public Class {
    public:
        Framebuffer();
        virtual ~Framebuffer();
    private:
        static const unsigned int _maxColourAttachments = 8;
    protected:
        bool _changed;
        ColourAttachmentHandle _colourAttachments[8];
        DepthAttachmentHandle _depthAttachment;
        ViewportHandle _viewport;
    protected:
        void updateViewport();
    public:
        virtual void bind();
        ViewportHandle getViewport();
        virtual void unbind();
        void validate();
    public:
        void setColourAttachment(const unsigned int index, const PixelbufferHandle pixelbuffer);
        void setDepthAttachment(const PixelbufferHandle pixelbuffer);
    public:
        static unsigned int getMaxColourAttachments() { return _maxColourAttachments; };
};

}
}

#endif
