# Renderer

### Goals

- [ ] Simple C++ project
- [ ] Draw Pixel
- [ ] Draw Line
- [ ] Draw Triangle
- [ ] Draw Filled Triangle
- [ ] ImGUI interface to change Triangle
- [ ] Draw Cube (Orthographic Projection)
- [ ] Rotate Cube
- [ ] Arcball camera
- [ ] Perspective Projection
- [ ] Basic lighting
- [ ] Texture mapping
- [ ] Backface culling
- [ ] Frustum culling

###### Non Goals

- Using external libraries for functionality (unless for performance)
  - SDL and ImGUI are the exceptions
- Performance initially. Clarity and simplicity are more important

### FAQ

**Why C++**

I don't really like C++ as it feels overly complex, but my attempts in
Go and C always end up with the need for a UI. I like ImGUI and C++ is
the easiest integration.

My previous project was in Go which was _fast enough_, but could be
faster. Also using ImGUI with the SDL backend wasn't implemented from
what I could find.

Another project was in C, which I like but I miss methods for
simplicity and not having vectors is a pain. Also using cimgui with
the SDL backend wasn't implemented from what I could find.

**Why Software Rendering (no OpenGL)**

Software rendering is more interesting to me and this is just for fun
and learning. The goal is to render Final Fantasy Tactics maps and
they are low polygon, so performance isn't really an issue.
