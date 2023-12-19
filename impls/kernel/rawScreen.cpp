#include "../../includes/kernel/rawScreen.hpp"

namespace GeoFrame {
namespace Kernel {
RawScreen::RawScreen(unsigned const &width, unsigned const &height)
    : mWidth(width), mHeight(height) {
    glGenFramebuffers(1, &mFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);
    unsigned rbo = 0;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mWidth,
                          mHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, rbo);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RawScreen::AddBuffer() {
    if (mTextures.size() >= 32) {
        M_GEO_THROW(KernelError, "Cannot add more than 32 buffers to screen.");
    }

    unsigned buffer = 0;

    glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);
    glGenTextures(1, &buffer);
    glBindTexture(GL_TEXTURE_2D, buffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, mWidth, mHeight, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0 + mTextures.size(),
                           GL_TEXTURE_2D, buffer, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Unique<RawTexture> texture =
        std::make_unique<RawTexture>(buffer, mWidth, mHeight, 4);
    mTextures.push_back(std::move(texture));
    glBindTexture(GL_TEXTURE_2D, 0);
}

void RawScreen::Transcript(RawScreen const &screen) const {
    this->ReadOnlyBind();
    screen.DrawOnlyBind();
    glBlitFramebuffer(0, 0, mWidth, mHeight, 0, 0, screen.mWidth,
                      screen.mHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    this->Unbind();
}

void RawScreen::DrawOnlyBind() const {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFrameBuffer);
    glDrawBuffers(mTextures.size(), COLOR_BUFFERS);
}

void RawScreen::Bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);
    glDrawBuffers(mTextures.size(), COLOR_BUFFERS);
}

void RawScreen::Unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK);
}
} // namespace Kernel
} // namespace GeoFrame
