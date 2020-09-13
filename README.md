# Beat Saber Map Generator
There are things computers can't do very well. Understand pacing and emphasis for example.
This approach is to translate keyboard input into notes. To leave some control to the user so the right notes get emphasis, and have breaks are where you want them to be.

Beat Saber Map Generator is a deterministic map generator. It has **factories** which produces notes, and **validators** that ensure the map's quality.
The most important validator, the `MatrixValidator` uses an 8 dimentional transition matrix describing every possible transition from one note to the next. The matrix is parsed from existing maps. This way, the user can give a their map the same "feeling" (to some degree) by giving it specific maps to build its matrix from.  
The matrix can be of type `bool` to describe whether a transition is legal. Or `float` describing the probability of one note transitioning the a spesific state. For a note in the matrix, the sum of the probabilities of all notes it can transition to is 1, making the `TransitionMatrix<float>` **a markov chain**.  


The plan is to have many kinds of factories to bring variety, along with many knobs and dials to change the outcome though config files. And many validators to cover the mapping errors that aren't picked up by implicitly the transition matrix validator among with edge cases (the maps used yo build the matrix may contain errors). 

## Factories
- Weighted Transition matrix. Build a markov chain with note transition pick random based on the weights going out of that node.
- Template. Has a collection of predefined patterns and puts them where they match the best.
- Weighted random. Pick the next note random based on some simple weights. More at the bottom, colors on theirs side mostly.
- Symmetrical random. Generate two notes at same timestamp the are symmetrical to a center plane or a center point.

## Validators
- Transition Matrix
- Double down
- Vision block
- Tangled arms
- Inverted colors. If inverted for too long
- Distance. If the distance to travel is inhuman.
- Single frame same direction. When several notes on same timestamp, they must be in a line.
- Bomb after note. If a bomb is in swing path after a note


## Installation
1. `sudo apt-get install libsfml-dev`
1. `git clone git@github.com:JonShard/BeatSaberMapGenerator.git`
1. `cd BeatSaberMapGenerator`
1. `make`
1. `./generator.out song.ogg`

## Usage
`generator.out [AUDIO FILE] [JSON ANNOTATION]`  

### Parameters
- `-a [MAP FILE...] [OPTIONAL --clean]` Analyse a map and build transition matrix. Stored in `binaryTransitionMatrix.data`. Optional falg `--clean` to discard existing matrix.
- `-g [JSON ANNOTATION]` Generate map from existing annotation file.

### Examples
`./generator.out songs/createOW/create.ogg`  
`./generator.out -a songs/createOW/ExpertPlusStandard.dat --clean`  
`./generator.out -g annotation.json`  
