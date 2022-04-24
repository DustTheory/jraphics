#ifndef ENGINE1_LIB_TEXTURE_H_
#define ENGINE1_LIB_TEXTURE_H_

#include <GLFW/glfw3.h>
#include <string>

#include "../lib/loggl.h"

namespace Engine1 {

class Texture {
    public:
        Texture(GLenum texture_target, const std::string &filename);
        bool Load();
        void Bind(GLenum texture_unit) const;
    private:
        std::string texture_filename_;
        GLenum texture_target_;
        GLuint texture_object_;
    };

} // namespace Engine1

#endif  // ENGINE1_LIB_TEXTURE_H_