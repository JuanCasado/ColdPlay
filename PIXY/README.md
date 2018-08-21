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
* Fast moving cheap cameras don't get well with object detecting algorithms. You can have an object perfectly detected by your robot, start moving towards it and suddenly IT HAS DISAPPEARED!!!. Your robot need to assume that the enviroment can change at a maximum range and objects don't teletransport jet so if in a certain frame you are seeing something and suddenly it's gone maybe you should check some more frames to make sure of it.
* Colors change, in a certain possition with a certain light red may be red but when you get closer suddenly red is purple!!!. You need to properly and frequently calibrate your equipment, light change though the day, though the field and even with your own pressence so avoid windows and beware of your shadow and always always calibrate right before a demo.
* Always check twice, whatever you are measuring do it twice, and make the average of the two measurements, you may need to change the action of your actuator four, at most cases five times a second. Use the remaining time to check you sensors and make the average of what they are seeing.

## Well defined state machines
ColdPlay is a moving state machine but only some changes along states are allowed, to change from an state to another I used accumulator that get filed by a certain rate is the a change state trigged was up and get emptyed by another rate if the trigger was down, this allow a high error tolerance.
Also a state machine can make your code easier to read.
