# Flappy Egg
<div style ="text-align: right"> Andrew Subach

 UVM CS2300A Fall 2023 
 
M4OEP</div>

## Overview 

This basic graphics game aims to recreate the trials and tribulations of the mid-2010s mobile classic Flappy Bird. Flappy bird was removed from both Apple and Android devices in 2014, leaving passionate fans without a way to flap.
With original copies of Flappy Bird on legacy devices going for exorbitant auction prices of over $20,000 on eBay and other vendors, there has arisen a void in the market of flap-based mobile games. 
Now, nearly a decade later the cult classic has returned, while simultaneously answering the age-old question of whether the chicken or the egg came first. It was the chicken, obviously. 

## Functionality

The project utilizes the OpenGL graphics libraries to render and display
the gameplay to the user. The user is a white egg-shaped block that continues flies forwards.
The motion of flight is simulated by a continuous left-motion of a finite repeating vector of randomly generated obstacles. 
The gravity is simulated by a constant downward motion of the user's egg. The space bar is the main control in the gameplay, which makes the egg
'flap' its wings and gain height to dodge obstacles. With the constant gravity and forward motion, the flight path results in arc which accurately portrays real-world flight.

The score is calculated by the number of rectangles that are to the left of the user, and the high score is stored in a text file, which is updated if the user exceeds the stored score. 
The biggest limitation of the pre-release version is that the score-saving feature only works on the developer's computer since absolute file paths were required to make the demonstration video. Further testing and development will solve this issue very soon. 



This project was adapted from the starter code for M4GP-Runner and M4GP-Confetti-Button, from which the following files were utilized:
<ul>
<li> color.h </li>
<li> color.cpp </li>
<li> debug.h </li>
<li> debug.cpp </li>
<li> font.h </li>
<li> font.cpp </li>
<li> fontRenderer.cpp </li>
<li> fontRenderer.h </li>
<li> shader.cpp </li>
<li> shader.h </li>
<li> shaderManager.cpp </li>
<li> shaderManager.h </li>
<li><strong>shapes Directory</strong></li>
<li><strong>res Directory</strong></li>
<li><strong>lib Directory</strong></li>
</ul>
</div>

Here is a [LINK](https://drive.google.com/file/d/1h9tKXvoJJUwhOKSupEtCIQnSwA0moV-X/view?usp=sharing) to the video demonstrating the functionality and gameplay. 
## Future Development

Future development will center around developing a system where the user can select various 'maps' that have various difficulties, as well as making the gameplay progressively harder
as the user's score increases. 

An unsolved bug at this time is that the user is not guaranteed to spawn in open-area, sometimes making it so the user reaches the end screen instantly. Furthermore, more testing is required to implement the score-saving feature of the game. Finally, while the game is functional it requires a laughably large amount of 
CPU time to run such a simple game. More development is needed to optimize the game processes and allow it to run in a more efficient manner. 

## Grading

Main Program Complexity: 40 pts

I believe my main program uses many concepts from Module 4, and is sufficiently complex for an Open-Ended project, especially one completed alone. There are only minor bugs present.

Keyboard/Mouse Input: 10 pts

I believe there is sufficient user input, given that this is a highly-interactive 2D game. 

C++ Concepts with Graphics: 25 pts

Use of graphics objects is well implemented, complex and is fitting for the objective of the program.
I use graphics concepts to simulate motion during gameplay, and use multiple screens to have a start and end screen. Furthermore, I
utilize the freetype library to display text to the user on screen. Finally, there is some file I/O concepts included to help store gameplay data. 

Comments/Documentation: 0 pts

Sufficient comments, and documentation in the README as well as in the code are present. 

Video: 0 pts

Video is present, linked above in this document. 

Lifespan: 0 pts

Project has a lifespan of over 7 days.

<strong>Total: 75 pts</strong> 



## References

[Open GL Documentation](https://registry.khronos.org/OpenGL-Refpages/gl4/)

[LearnOpenGL.com](https://learnopengl.com/Getting-started/OpenGL)

Prof. Lisa Dion

