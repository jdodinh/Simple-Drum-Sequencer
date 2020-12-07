# Simple-Drum-Sequencer
Basic 10 channel drum sequencer interface between Arduino and Max/MSP to mimic a Roland tr-909 drum machine.

## Introduction
My interest with drum machines happened by coincidence. I quite frequently listen to electronic music, and I would especially consider myself to be a fan of Daft Punk. I have come to like their track 'Revolution 909' from their 'Homework' album, whose title I previously didin't give much tought to. Upon finding out, that the title actually references the Roland tr-909 Rhythm Composer (Drum machine) I started reading further on the topic and researching Demos/Tutorials. I decided that if I manage to obtain a set samples from this drum machine I would be able to construct a miniature version of the drum machine using the development tools learned in the MUMT 306 class.

With the limited number of pins on the Arduino and little overall space for button placement the goal of the project was to create a simple sequencer that can provide the vital functionalities of a full drum machine, while using the smallest amount of controls possible. The result is a drum machine that uses 5 buttons and 1 knob. The following sections decribe the functionality of the project, the project development process as well as comments on obstacles faced and areas of improvement/development.

## Project overview and development

The overall product demonstrated can very clearly be divided into two parts. The heavy lifting is done by the Max/MSP patch, while the Arduino acts as a physical/visual interface used to have an adequate user experience for controlling the sound. The two parts and their functionalities are described below.

### Max/MSP patch
The Max/MSP component of the project has two distinguishable elements, namely the Drum Machine patch (`Drum_Machine_v1_4`), and the Drum Machine Controller (`DM_controller`). They are described below:

#### Drum Machine Patch
To begin with the Drum_Machine patch (version 1.4 used most recently) provides the essence of the drum machine elements. It is loaded as an object into the DM_controller and contains 8 inlets, and 1 outlet. The outlet simply outputs sounds that can be directed straight into a `dac~` object. 6 out of the 8 inputs are used to control the sequencing of the drum machine. They are listed below (functionalities of inlets 7&8 are described in the next section):
1. The first inlet controls the tempo of the sequencing in beats per minute. This value can be set from 60 to 200.  This inlet is directly connected to a Max tempo object as the tempo argument. The Tempo object further has default 16 divisions of a whole note, and a beat multiplier of 1. 
1. The second inlet turns the tempo object on or off, and thus is a toggle for the sequencer. 
1. The third inlet controls the drum channel selected. It cycles from 0 to 10, with 0 being a default empty channel
1. The fourth inlet controls the beat index (from 1 to 16) that is to be selected within a specified channel (chosen by input 2). It again cycles from 1 to 16, and allows a selection of a particular beat within a particular channel
1. The fifth inlet turns on/off a beat which corresponds to the selection made using inlets 3 and 4. 
1. The last inlet finally allows a reset to an empty drum sequence. It deletes all previously turned on beats on all channels.

Within the Drum machine patch, we have 10 channels, each represented by separate (yet identical) subpatches. The design is such that only one subpatch (channel) can be controlled at a time, with that subpatch defined by the channel index chosen using inlet 3. This subpatch further contains a custom `drum_generator` object which contains the control of the indices of the beats turned on for this particular channel. This `drum_generator` is shown below:

As seen in the above figure, the topmost select object controls the index of the beat which is to be turned on (essentially which one of the 16 toggle buttons to trigger). This is controlled by the fourth inlet of the Drum Machine patch, as a result redirects the bang (triggered by inlet 5) to the desired outlet in the select object, trigerring the corresponding bang. Once the toggle is on, the corresponding gate object freely passes bangs from the tempo object at the given beat, which triggers the groove~ object containing the preloaded drum sample.

#### DM_controller

The Drum Machine controller provides three vital higher level features related to the operation of the drum machine. 

To begin the controller provides the 6 necessary inputs (described above) for the basic execution of the Drum Machine. A slider is used for input 1 (tempo select), a toggle button is used for the control of the tempo object, and buttons are used for the remaining 4 inputs. 

The drum machine patch is set up to have default samples preloaded for each one of the channels. Thus another high level functionality of the DM controller is the ability to change the sample that is preloaded on each channel.  A `radiogroup` object allows for the choice of which channel to modify. Following the channel selection one can press any one of the 81 samples, to change the sample loaded on the particular channel. One can also reset to defaults using the using the toggle button on the left of the channel selection. 

Finally the controller makes use of the serial communication between the Arduino and Max/MSP. We obtain the 6 controls from the Arduino directly into Max, which are then used to control the Drum Machine object and allow for the physical user interface. 

### Arduino communication


## Hurdles and limitations
The project development process went relatively smoothly. The separate nature of the Max/MSP patch and the Arduino allowed for the two parts to be developed at their own pace, without the need for too much dependency. Again, the role of the Arduino was simply to make the interface visual, and only 6 types of messages are sent from the Arduino to the Max Patch. 

The max patch was developed first, with of course the creation of the drum_generation object. The patches were then built from bottom up, with subpaching used whenever the individual elements became too complicated. Send/receive objects were also used to allow for cord-less connections. 

Version numbers were used for the drum machine and the Arduino programs to ensure that attempts to change aspects of the program didn't result in sudden bugs/unsolvable mistakes. 

A particular problematic element is the potentiometer that came with the Arduino. It seems that the connection is very unreliable, and thus usually needs to be held in order for the output to be steady. I tried to connect is using wires, instead of into the breadboard (in order to assure a more stable contact), however it resulted in its pins breaking. I ended up managing to connect the potentiometer by scraping away some plastic covering the two outputs, and supergluing some wires, which allows for a relatively relatively stable output signal. For demonstration purposes the communication might need to be turned off. 

## Improvement strategies

Further details regarding the control of the channels individually, such as blending the sounds for the different drums, and adding an eq. Controlling the volume of the channel in the final mix would also be exteremly useful.

Adding a shuffle object to split up beats

Uniting the arduino and max msp, such that the use of the max msp patch is not vital during the execution.

## Conclusion


