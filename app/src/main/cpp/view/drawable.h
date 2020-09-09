//
// Created by samuel on 08/09/20.
//

#ifndef PHYVR_NDK_DRAWABLE_H
#define PHYVR_NDK_DRAWABLE_H

struct gl_infos {

};

class Drawable {
public:
    virtual void draw(gl_infos) = 0;
};

#endif //PHYVR_NDK_DRAWABLE_H
