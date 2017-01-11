Drawing
-------

- Begin Command Buffer
    - Acquire a swapchain buffer and the image to render into.
    - Begin a render pass 
        - Bind the pipeline
        - Bind the descriptor set
        - Bind the Vertex Buffer
        - Set the viewport
        - Set the scissor rectangle
        - draw the vertices
    - End Render Pass
- End Command Buffer
- Create Fence
- Submit Command Buffer
- Wait Completion
- Present Swapchain Buffer to Display



