```plantuml
!pragma teoz true

title
Detailed design of "CGU"
end title

actor "Client" as client

box "External" #MOTIVATION
participant "Handler" as handler

box "Internal" #Lavender
participant "Converter" as converter
participant "Processor" as processor
participant "IO" as io
participant "Window" as window
end box

end box

opt "handlers"
opt "encode"
handler -> converter: convert from the given origin image type to CGU
converter -> processor: perform sub operations to complete conversion operation
converter -> handler: updated image surface 
handler -> io: validate and write down metadata to the output file
io -> handler: backed metadata 
handler -> io: write down update image surface as a BMP
end

opt "decode"
handler -> io: read given image file
io -> handler: backed image surface
handler -> io: decerialize file metadata to retrieve conversion type
io -> handler: decerialized file metadata structure
handler -> converter: convert from the CGU to the requested image type with the specified conversion type
converter -> processor: perform sub operations to complete conversion operation
converter -> handler: updated image surface 
handler -> io: write down update image surface as an image of the requested type
end

opt "view"
handler -> io: read given image file
io -> handler: backed image surface
handler -> io: decerialize file metadata to retrieve conversion type
io -> handler: decerialized file metadata structure
handler -> window: open window with the given image surface
end
end

opt "commands"
opt "encode"
client -> handler: perform encode operation of the given input image\nwith the mentioned conversion type
end
opt "decode"
client -> handler: perform decode operation for the given image in CGU format
end
opt "view"
client -> handler: open window to show the image with the given location
end
end
```