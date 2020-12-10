# Simple-Drum-Sequencer
Basic 10 channel drum sequencer interface between Arduino and Max/MSP to mimic a Roland tr-909 drum machine.

## Introduction
My interest with drum machines came to me quite coincidentally. I quite frequently listen to electronic music, and I would especially consider myself to be a fan of Daft Punk. I have come to like their track 'Revolution 909' from their 'Homework' album, whose title I previously didin't give much thought to. Upon finding out, that the title actually references the Roland TR-909 Rhythm Composer (Drum machine) I began reading further on the topic and researching Demos/Tutorials. I decided that if I manage to obtain a set samples from this drum machine I would be able to construct a simpified drum machine emulator using the tools acquired throughout the MUMT 306 course.

With the limited number of pins on the Arduino and little overall space for button placement the goal of the project was to create a simple sequencer that can provide the vital functionalities of a full drum machine, while using the smallest amount of controls possible. The result is a drum machine that uses 5 buttons and 1 knob. The following sections decribe the project's technical details, the development process including difficulties faced and areas of improvement/development.

## Project overview and development

The overall product can very clearly be divided into two parts. The heavy lifting is done by the Max/MSP patch, while the Arduino acts as a physical/visual interface used to have an adequate user experience for controlling the sound. The two parts and their functionalities are described below.

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
![Alt text](images/drum_generator.png?raw=true "Title")
As seen in the above figure, the topmost select object controls the index of the beat which is to be turned on (essentially which one of the 16 toggle buttons to trigger). This is controlled by the fourth inlet of the Drum Machine patch, as a result redirects the bang (triggered by inlet 5) to the desired outlet in the select object, trigerring the corresponding bang. Once the toggle is on, the corresponding gate object freely passes bangs from the tempo object at the given beat, which triggers the `groove~` object containing the preloaded drum sample.

#### DM_controller

The Drum Machine controller provides three vital higher level features related to the operation of the drum machine. 

To begin the controller provides the 6 necessary inputs (described above) for the basic execution of the Drum Machine. A slider is used for input 1 (tempo select), a toggle button is used turn the tempo object on/off, and buttons are used for the remaining 4 inputs. 

The drum machine patch is set up to have default samples preloaded for each one of the channels. Thus another high level functionality of the DM controller is the ability to change the sample that is preloaded on each channel.  A `radiogroup` object allows for the choice of which channel to modify. Following the channel selection one can press any one of the 80 samples, to change the sample loaded on that particular channel. One can also reset to defaults using the toggle button on the left of the channel selection. 

It is important that the audio samples are accessible to the max patch. In the case of this project, those samples were named in the particular ways, which can be seen by all the `read` messages in the patch. For copyright purposes, the samples are not included in this repository. Therefore to use the drum machine the user needs to provide his own drum samples, and change the name of the sample adequately so the patch can load it. 

Finally the controller makes use of the serial communication between the Arduino and Max/MSP. We obtain the 6 controls from the Arduino directly into Max, which are then used to control the Drum Machine object and allow for the physical user interface. 

### Arduino communication

The arduino communication necessitated only the use of 6 different types of messages, which facilitated the overall structure of the communication. The analog reading of the potentiometer was scaled to a range of 60-200, and were perpetually sent to the Max patch to adjust the playback speed. Finally 5 buttons were connected to 5 digital pins, and their values were read at each iteration of the loop. 

If a button was pressed, the execution of the loop was halted until the button was de-pressed, following which the corresponding actions were carried out. Therefore the stream of messages sent to Max consisted solely of values 1, 2, 3, 4, 5 (which then was read as an instruction to pass a bang to a particular `DM_controller` inlet), and a value representing the tempo (in the range 60-200bpm).

The Arduino program kept internal variables representing the status of the channels given the way the keys were pressed. To name a few this consisted of a variable keeping track of the current channel, the beat index, sequencer on/off state as well as a 10 by 16 binary array keeping track of that beats are turned on at which channels. 

The `LiquidCrystal` library was used to make use of the LCD, which allowed a very simple interface for writing data to the display. Luckily, the LCD provided with the arduino contains exactly 16 columns, which is ideal for the display of the status of the channels. 

To summarize each loop iteration consisted of three main components:
1. Sending a message to Max (1, 2, 3, 4, 5 or tempo value)
2. Updating internal variables to reflect the changes
3. Updating the display to represent the changes

## Hurdles and limitations
The project development process went relatively smoothly. The separate nature of the Max/MSP patch and the Arduino allowed for the two parts to be developed separately and at their own pace, without the need for too much dependency. Again, the role of the Arduino was simply to make the interface visual, as only 6 types of messages are sent from the Arduino to the Max Patch. 

The max patch was developed first, with of course the creation of the `drum_generator` object. The patches were then built from bottom up, with subpaching used whenever the individual elements became too complicated. Send/receive objects were also used to allow for cord-less connections. 

Version numbers were used for the drum machine and the Arduino programs to ensure that attempts to change aspects of the program didn't result in sudden bugs/unsolvable mistakes, and that there was always a version to fall back on.

A particular problematic element was the potentiometer that came with the Arduino. It seems that the connection was very unreliable, and thus usually needed to be held in order for the output to be steady. I tried to connect is using wires, instead of the breadboard (in order to assure a more stable contact), however it resulted in its pins breaking. I ended up managing to connect the potentiometer by scraping away some plastic covering the two outputs, and supergluing some wires, which allows for a relatively relatively stable output signal. For demonstration purposes the communication might need to be turned off. 

## Improvement strategies
There are several strategies for improvements which seem to be natural paths for the continued development of the project. 

To begin with, it would've been particularly usedul to add further functionalities to the control of the channels individually. One example would be volume control for each drum, which would allow for the mixing of the separate drum components. Another good idea would be to add an EQ control for each drum channel, which would've involved more complex aspects of signal processing. This would've taken the product a step closer to being versatile enough to truly be used in a professional setting.

There are also other functionalities of real drum machines that could be integrated to add more complexity. One such aspect is the choice of a shuffle. Choosing a shuffle option would result in the possibility to give the beats a swing element, and thus allow for an uneven breakdown of each full note paving the way to a wider range of music being produced using the machine.

Finally, the ultimate goal would be to make the entire product a stand alone object, without the need for a computer connection. This would involve the preloading of the max patches, along with all the samples into the Arduino (which I'm not even sure would be possible with the hardware at hand) and uniting the two separate elements into one thing. It would've demonstrated to be particularly useful if we could simpy avoid any connection to the computer and have the drum machine work on its own, in the form of a mini calculator. 

## Demonstration
Please find [here](https://youtu.be/uhJZyoTEWLM) a demonstration to the little Drum machine in use.

## Conclusion
To conclude, the product provides a very simple interface for the creation of different drum beats, and the development filled me with great satisfaction. I gained a lot of insight into how such devices are build in the back end, and most of all I gained a lot of appreciation for Max/MSP in terms of its utility for sound production/engineering purposes. I am happy with the results, and how the physical interface can create the beats very simply. All this being said, I also see many ways in which I can improve the project, and would be happy to attempt to further develop certain aspects discussed out of personal curiosity. 
