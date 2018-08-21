# Final project 
At the final stage of the project we changed it's theme from a simple object detector to a something a bit more useful a recycling robot.
Our robot cold detct objects by its colour and put them in a container with other colour, this basic idea could be applied to recycling.
The things we buy could have a coloured stripe that robots could detect and use them to recicle them.
This are some final thoughts that we had at the end of the project. Anyway here it commes some bit more details about the ColdPlay robot code.

## Artificial vision is hard
Working with artificial vision means high error margins. Whatever you do you always need to have in mind:
* Colour is not only produced by the objects. If you are in a compleately dark room i bet you can distinguish between green and blue, your robot won't be able to do so either.
Light is the most important thing when you are working with artificial vision, you need to create a proper environment for your robot with homogeneous white light along the hole working field. Also you my need to consider to isolate your working enviroment with white walls because if you are trying 
to detect a red ball wearing a red T-shirt it may be just a bit harder.
