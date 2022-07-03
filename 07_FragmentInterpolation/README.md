## Video
* [Fragment Interpolation - Vulkan Game Engine Tutorial 07](https://www.youtube.com/watch?v=ngoZZkMuCOM)

## Intro
Essentially computer monitors operate in a logarithmic/non-linear color space and apply a gamma of roughly 2.2, which has the effect of darkening midpoint values. Note that black stays black, and white stays white. It’s only all the values in between that are affected. 
 
Here’s a good explanation: https://www.cambridgeincolour.com/tutorials/gamma-correction.htm

All of our interpolation and color blending operations within the graphics pipeline need to occur in a linear color space to be correct. Because otherwise if you try to linear interpolate two colors in a non-linear space, you will get something with incorrect brightness. The vulkan rendering pipeline operates in a linear color space.

video by minute physics demonstrating this: [Computer Color is Broken](https://www.youtube.com/watch?v=LKnqECcg6Gw)

Ok but then why do things like photos appear correctly when viewed on your monitor? Well, all images have been “gamma corrected”, meaning that the image file has been saved with values adjusted in the opposite direction. So when you take a gamma corrected image, and display it on a typical monitor with a gamma of 2.2, they cancel each other out, and you get the correct colors being displayed.

### Why do we go through this hassle
Why is this even a thing? Couldn’t we store our images linearly, and display linearly. And ya that would work but human vision is much better and distinguishing darker shades. Computer memory is limited, for example 8 bits per pixel would mean only 256 different shades from dark to light would be available. We need to allocate more of our potential memory to darker values since we can distinguish them better, otherwise we’re kind of just wasting memory on things we can’t even tell apart. 


### So what’s happening with vulkan

So right now if we render a scene, all our rendering occurs in a linear color space. Then when our monitor takes the image and displays it applies gamma, darkening all our midtones.
By changing the format from UNORM to SRGB tells Vulkan that when it writes output to the framebuffer’s color attachment, to automatically apply this gamma correction.

So essentially vulkan renders linearly -> writes to our framebuffer and lightens our values automatically -> monitor displays values darkening them -> correct result

 Honestly right now this doesn’t matter much, and many older graphics engines completely ignored gamma correction and still look great. But once we start dealing with more realistic rendering techniques, we need to account for gamma if things are to appear physically accurate.


## Links
* [Barycentric coordinate system](https://en.wikipedia.org/wiki/Barycentric_coordinate_system)
