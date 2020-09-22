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
- Random. Pick the next note random based on some simple weights. More at the bottom, colors on theirs side mostly.
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
Note that right now the generator requires a BPM of 120 in the info file as the program has no way to get this right now.  
`generator.out [AUDIO FILE] [JSON NOTATIONS]` 

### Parameters
- `-a [MAP FILE...] [OPTIONAL --clean]` Analyse a map and build transition matrix. Stored in `binaryTransitionMatrix.data`. Optional falg `--clean` to discard existing matrix.
- `-g [JSON NOTATIONS]` Generate map from existing keyframes file. Optional parameter `--seed 42`.

### Examples
`./generator.out songs/createOW/create.ogg /songs/createOW/keyframes.json`  
`./generator.out -a songs/createOW/ExpertPlusStandard.dat --clean`  
`./generator.out -g keyframes.json`  

### Config File
Config file `config.json` is loaded from the same location as the program.  
Format:
```
{
    "generator": {
        "noteClusterTime": float - Time that defines how close notes have to be to be considered in the same cluser or "frame" (seconds).
        "factories": {
            "maxAttempts": int - The maximum amount of attempts to generate from note A, a valid note B before backtracking by removing note A.
            "randomFactory": {
                "enabled": bool - Is the random factory enabled.
            }
            "symmetricalFactory": {
                "enabled": bool - Is the symmetrical factory enabled.
                "onlyOnBorder": bool - Disallow notes to be generatred in the two ceneter positions.  
            }
        },
        "validators": {
            "validateTimeAfterNote": float - Time after a note in which the validator will care if an illegal note is placed (seconds).
            "matrixValidator": {
                "enabled": bool - Is the matrix validator enabled.
                "binaryMatrixFilePath": string - Path to ascii file containing binary transition matrix containing only 1, 0 and whitespace.  
            },
            "doubleDownValidator": {
                "enabled": bool - Is the double down validator enabled.
                "angleToBeDoubleDown": The difference in angles two notes have to be for theirs transition to qualify as a double down. Possible values: 0, 45, 90, 135. 180 would deny all notes always.
            }
        }
    },
    "editor": {
        "windowWidth": int - Amount of pixles of the editor window in X direction.
        "windowHeight": int - Amount of pixles of the editor window in Y direction.
    }
}
```
Example:
```json
{
    "generator": {
        "noteClusterTime": 0.05,
        "factories": {
            "maxAttempts": 10000,
            "randomFactory": {
                "enabled": true
            },
            "symmetricalFactory": {
                "enabled": true,
                "onlyOnBorder" : true
            }
        },
        "validators": {
            "validateTimeAfterNote": 2,
            "matrixValidator": {
                "enabled": false,
                "binaryMatrixFilePath": "binaryTransitionMatrix.data"
            },
            "doubleDownValidator": {
                "enabled": true,
                "angleToBeDoubleDown": 90
            }
        }
    },
    "editor": {
        "windowWidth": 1600,
        "windowHeight": 900
    }
}
```

### Editor Keybindings
Create a keyframe by pressing one or several keys in the range A-Z + 0-9. Pressing several at a time will register the keyframe with several concurrent presses with a **max of 4**, indicating the intensity of that keyframe.  
| Key         | Action        |
|-------------|---------------|
| Space       | Play / pause  |
| F5          | Save notation |
| F10         | Generate map  |
| Scroll      | Generate map  |
| Backspace   | Go to start   |
| Numpad+     | Zoom in       |
| Numpad-     | Zoom out      |
| Page up     | Speed up      |
| Page down   | Speed down    |

