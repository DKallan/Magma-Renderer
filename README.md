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
At least planning to implement a z-buffer to make sure everything is rendered correctly.
