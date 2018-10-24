# Magma-Renderer
This repository contains my C++ software renderer, which I had to make for a school exercise (for the Game Development Minor I followed).

### Current State of the renderer ###
I am able to render simple models using a vertex- and index buffer. It is possible to fill the vertices of this model using a given color.
The color may be specified within the vertex buffer. There are a few settings which can toggle the RenderMode (lines, filled or both), the
ViewMode (orthographic or perspective) and the line colors (normal or inverted).
```
RenderMode:
  lines  > This is the basic wireframe mode, meaning it will only draw lines between vertices.
  filled > This is the basic filled mode, it will fill the triangles with a specified color.
  both   > This is a combination of the previously mentioned modes, 
           first filling all triangles and then drawing the wireframe on top.
```
```
ViewMode:
  perspective  > Render the objects with perspective.
  orthographic > Render the objects without perspective.
```
```
LineColor:
  normal   > Draw the lines using the given colors from the vertex buffer.
  inverted > Draw the lines with inverted colors. (subtract the given colors from white). 
             This allows the lines to be more visible on color.
```

### Planned ###
- At least planning to implement a z-buffer to make sure everything is rendered correctly.
- Optimize the EdgeEquation using block based rendering, although I need to do some research on that (I do not want to blindly copy some tutorial like trenki2's, without understanding it fully).

### Literature used ####
I obviously did not re-invent the wheel, nor did I come up with all code myself. I will post the links I have used here:

(1) https://learnopengl.com/Getting-started/Coordinate-Systems

(2) https://trenki2.github.io/blog/2017/06/06/developing-a-software-renderer-part1/ 

(3) http://www.cs.unc.edu/~blloyd/comp770/Lecture08.pdf 

(4) http://groups.csail.mit.edu/graphics/classes/6.837/F98/Lecture7/triangles.html 

1. I used this mainly for the matrices and understanding them.
2. I used this for the basics of Edge Equations.
3. I used this for information about rasterization.
4. I used this aswell for some info on Edge Equations and code examples.
