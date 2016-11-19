#Vulkan Initialization

This is the sequence of steps required to setup Vulkan window

- initialize vulkan instance
    - requires
        - global properties
        - extensions
        - layers
        - application identification information
    -  set
        - vulkan instance
- identifies vulkan devices
    - requires
        - 
        - 
    - set
- enumerate queues
    - requires
        - vulkan device
    - set
        - count queues
        - discover presentation queue
- setup a window (Platform Specific)
    - requires
    
- identifies queues
    - requires
        - queue enumeration
    - set
        - index of graphic queue
        - index of presenter queue
        
- identifies supported format
    - requires
                  
- setup a surface

- setup swap chain
    - init swap chain info
        - requires
            - surface
            - image format
            - 2d extent
        - set
            - swap chain info
    - setup swap chain
        - requires
            - swap chain info
        - set
            - swap buffers
        
 
- create images
    - requires
        - device
        - buffers
        - (?bitmap) image
    - set
        - image x buffer
    