<img width="640" height="480" alt="demo-640x480" src="https://github.com/user-attachments/assets/31538af1-c719-4cc2-b7ee-fe25c9ac0a9b" />

fbinter, short for framebuffer interactor, is a project started from boredom.
Its original name is fbfun, which stands for framebuffer fun. References to this
name can still be found in the source code, where every header guard is prefixed
with `_KT_FBFUN_`.

# What it is and what it does
fbinter is a software rendering tool which has functions to draw lines,
parametric curves, lines and parametric curves in 3D, and filled circles. It
uses the `/dev/dri/card*` interfaces to render in place of `/dev/fb*` despite
what the name fbinter may suggest, since it is faster to use `/dev/dri/card*`.
It also has a built-in `wframe` type which lets users create 3D wireframes and
there also exists a function to rotate the wireframes in a given plane of
rotation. This system can be expanded with 4D wireframes as demonstrated in
`example_hyper.c`.

# Why it is
fbinter exists partly because I was bored, partly because I wanted to see if I
could make my own sort of rendering library using `/dev/fb0` after learning
about its existence.

# Things I want to implement
- Filled 3D triangle rendering

# Documentatiom
*(I haven't written documentation yet)*
