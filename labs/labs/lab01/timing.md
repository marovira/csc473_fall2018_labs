# Timing in Animation

## Overview
The purpose of this lab is to explore the concept of time in terms of
simulation. Instructions:

* Download the code to your desktops,
* Open a command line, and navigate to the directory. Then type:
``` sh
mkdir build
cd build
cmake ..
make
```
* If everything works correctly, the code should build. You can run the lab code
  with `./labs/lab01/lab01`

You are allowed to use whatever text editor you prefer. The files that we'll be
looking at are the following:

* `labs/labs/lab01/include/Planet.hpp` and `PlanetScene.hpp`
* `labs/labs/lab01/source/Planet.cpp` and `PlanetScene.cpp`

## Task 1: Making Earth Spin
As is, the model is not very interesting, so go to `Planet.cpp` and fill in the
code for the following function:

```c++
void Planet::updateGeometry(atlas::core::Time<> const& t)
{

}
```
You can build a simple rotation matrix using
`glm::rotate(atlas::math::Matrix4(1.0f), glm::radians(degrees),
atlas::math::Vector());`. It may help to look at the documentation for `Time`
which can be found
[here](https://marovira.github.io/atlas/classatlas_1_1core_1_1_time.html). Play
around with different values. Can you make the planet spin any faster? Slower?

## Task 2: A more consistent spin
As you might've guessed, using arbitrary values to spin the planet is not the
best of ideas in terms of portability. If we were to move computers, we may need
to change the value of the scaling factor for the spin. The question is: can we
find a better way of handling this? 

It turns out we can. Consider the general structure of the main-loop in any
graphics application:

```c++
while (true)
{
    UpdateAnimations(delta);
    Render();
    // Other stuff
}
```

Of particular interest is the fact that the render loop invokes both the
animation engine and the render functions sequentially. This means that they
will be called as fast as the loop can run. In this case, we can simply write an
`if`-statement to control when the animation gets updated. Modify
`PlanetScene.cpp`, in particular look at `updateScene` and write the code so
that the animation only occurs 20 times per second. In pseudo-code, you should
have something like this:

```c++
tickCount = 0;                      // Global or member variable.
rate = ?;                           // Compute the value so we update 20 times per second.
if (tickCount == rate && mPlay)     // Notice that your counter may go over the rate. How can we fix this?
{
    mPlanet.updateGeometry(mTime);
}
// Update your tickCount here. Use mTime.deltaTime;
```

## Task 3: Using an FPS counter
A more consistent way of timing events is to use frame rate of the application.
As you can tell from the FPS counter in the GUI, our refresh rate is roughly
60 FPS. So what if we made our animation run at 60 FPS? Modify your code so that
the animation gets updated at 60FPS. Replace the code you wrote for the previous
task with the `mAnimCounter` member variable. The documentation for the FPS
counter can be seen
[here](https://marovira.github.io/atlas/classatlas_1_1utils_1_1_f_p_s_counter.html).

As a general rule of thumb, the animation engine needs to run faster than the
rendering code. The logic being that the animation engine requires smaller time
steps in order to improve precision (we will see why next lab), while the
rendering engine can be updated more slowly in order to save CPU/GPU cycles.
This would work if our applications weren't locked at 60FPS, however we will
still see what the effects of this are. Modify `PlanetScene.cpp` such that:

* The animation engine runs at 60FPS, and
* The rendering engine runs at 30FPS.

The result should stutter a bit, but overall the animation looks smooth. This is
the way that most game engines are structured and it is the general rule of
thumb that we will follow for this class. If your refresh rates are locked at
60FPS, it may be possible to simply update both engines at the same speed, but
in general this is not the case.

