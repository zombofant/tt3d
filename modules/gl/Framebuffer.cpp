/**********************************************************************
File name: Framebuffer.cpp
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
#include "Framebuffer.hpp"
#include <boost/format.hpp>
#include "modules/utils/Exception.hpp"

namespace tt3d {
namespace GL {
    
using namespace Utils;

static const int depthFormatCount = 5;
static const GLenum depthFormats[depthFormatCount] = {
    GL_STENCIL_INDEX,
    GL_STENCIL_INDEX1,
    GL_STENCIL_INDEX4,
    GL_STENCIL_INDEX8,
    GL_STENCIL_INDEX16
};

static const int stencilFormatCount = 4;
static const GLenum stencilFormats[stencilFormatCount] = {
    GL_DEPTH_COMPONENT,
    GL_DEPTH_COMPONENT16,
    GL_DEPTH_COMPONENT24,
    GL_DEPTH_COMPONENT32
};

/* tt3d::GL::Attachment */

Attachment::Attachment(const PixelbufferHandle pixelbuffer):
    _pixelbuffer(pixelbuffer),
    _attachFunc(pixelbuffer->getAttachFunc()),
    _attachKind(pixelbuffer->getBufferKind())
{
    
}

void Attachment::attach(const GLenum target) 
{
    _attachFunc(GL_FRAMEBUFFER, target, _attachKind, _pixelbuffer->getID());
}

bool Attachment::isDepthFormat(const GLenum format)
{
    for (int i = 0; i < depthFormatCount; i++) {
        if (format == depthFormats[i])
            return true;
    }
    return false;
}

bool Attachment::isStencilFormat(const GLenum format)
{
    for (int i = 0; i < stencilFormatCount; i++) {
        if (format == stencilFormats[i])
            return true;
    }
    return false;
}

/* tt3d::GL::DepthAttachment */

bool DepthAttachment::isCompatible(const PixelbufferHandle pixelbuffer)
{
    return isDepthFormat(pixelbuffer->getFormat());
}

/* tt3d::GL::ColourAttachment */

bool ColourAttachment::isCompatible(const PixelbufferHandle pixelbuffer)
{
    return !isDepthFormat(pixelbuffer->getFormat()) && !isStencilFormat(pixelbuffer->getFormat());
}


/* tt3d::GL::Framebuffer */

Framebuffer::Framebuffer():
    Class::Class(),
    _changed(true),
    _colourAttachments(),
    _depthAttachment(),
    _viewport()
{
}

void Framebuffer::updateViewport() 
{   
    validate();
    for (unsigned int i = 0; i < _maxColourAttachments; i++) {
        const ColourAttachment *attachment = _colourAttachments[i].get();
        if (attachment) {
            if (!_viewport.get()) {
                _viewport = ViewportHandle(new Viewport());
            }
            _viewport->setWidth(attachment->getWidth());
            _viewport->setHeight(attachment->getHeight());
            return;
        }
    }
    const DepthAttachment *attachment = _depthAttachment.get();
    if (attachment) {
        if (!_viewport.get()) {
            _viewport = ViewportHandle(new Viewport());
        }
        _viewport->setWidth(attachment->getWidth());
        _viewport->setHeight(attachment->getHeight());
        return;
    }
    if (_viewport.get()) {
        _viewport = ViewportHandle();
    }
        
}

void Framebuffer::bind() 
{
    glBindFramebuffer(GL_FRAMEBUFFER, glID);
}

ViewportHandle Framebuffer::getViewport() 
{
    updateViewport();
    return _viewport;
}

void Framebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::validate()
{
    bind();
    if (_depthAttachment.get()) {
        _depthAttachment->attach(GL_DEPTH_ATTACHMENT);
    }
    for (unsigned int i = 0; i < _maxColourAttachments; i++) {
        if (_colourAttachments[i].get()) {
            _colourAttachments[i]->attach(GL_COLOR_ATTACHMENT0 + i);
        }
    }
    
    GLenum errno;
    errno = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch (errno) {
        case GL_FRAMEBUFFER_COMPLETE: {
            break;
        };
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: {
            throw FramebufferError("Incorrect attachment");
        };
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: {
            throw FramebufferError("Missing attachment");
        };
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT: {
            throw FramebufferError("Incorrect dimensions");
        };
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: {
            throw FramebufferError("Incorrect draw buffer");
        };
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: {
            throw FramebufferError("Incorrect read buffer");
        };
        case GL_FRAMEBUFFER_UNSUPPORTED: {
            throw FramebufferError("FBOs / FBO status check unsupported");
        };
        default:
            throw FramebufferError((boost::format("Unknown framebuffer error %d") % errno).str());
    }
    unbind();
}

void Framebuffer::setColourAttachment(const unsigned int index, const PixelbufferHandle pixelbuffer)
{
    if ((index < 0) || (index >= _maxColourAttachments))
        throw IndexError(index, 0, _maxColourAttachments-1);
    if (!pixelbuffer.get()) {
        _colourAttachments[index] = ColourAttachmentHandle();
    } else {
        if (!ColourAttachment::isCompatible(pixelbuffer)) {
            throw FramebufferError("Attempt to attach an incompatible pixelbuffer as colour attachment.");
        } else {
            _colourAttachments[index] = ColourAttachmentHandle(new ColourAttachment(pixelbuffer));
        }
    }
}

void Framebuffer::setDepthAttachment(const PixelbufferHandle pixelbuffer)
{
    if (!pixelbuffer.get()) {
        _depthAttachment = DepthAttachmentHandle();
    } else {
        if (!DepthAttachment::isCompatible(pixelbuffer)) {
            throw FramebufferError("Attempt to attach an incompatible pixelbuffer as depth attachment.");
        } else {
            _depthAttachment = DepthAttachmentHandle(new DepthAttachment(pixelbuffer));
        }
    }
}

}
}
