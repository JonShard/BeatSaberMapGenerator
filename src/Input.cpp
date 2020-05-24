#include "../include/OK/Input.hpp"

namespace OK {


int Input::indexOfKey(sf::Keyboard::Key key) {

    auto it = std::find_if(s_keys.begin(), s_keys.end(),
                        [key]
                        (const std::pair<float, sf::Event::KeyEvent> & x)
                        {return key == x.second.code;});
            
    if (it == s_keys.end()) {
        return -1;
    }
    return std::distance(s_keys.begin(), it);
}

bool Input::isKeyDown(sf::Keyboard::Key key) {
    auto it = std::find_if(s_downKeys.begin(), s_downKeys.end(),
                        [key]
                        (const sf::Event::KeyEvent & x)
                        {return key == x.code;});
    return (it != s_downKeys.end());
}

void Input::update(float dt, sf::RenderWindow & w) {
    sf::Event event;
    s_downKeys.clear();
    for (auto & k : s_keys) {           // Add time to keys that are not shelved.
        if (k.first >= 0) k.first += dt;
    }

    for (auto & k : s_keys) {           // If a single key has been down for keyWait, add all active key to down list, shelve all those keys.
        if (k.first > c_downKeyWait) {  
            for (auto & j : s_keys) {
                if (j.first >= 0) {
                    s_downKeys.push_back(j.second);
                    j.first = -1;
                }
            }
            break;
        }
    }

    for (auto & k : s_downKeys) {  
        printf("DownKey: %d \n", k.code);
    }
    // for (auto & k : s_keys) {  
    //     printf("Key: %d \ttime: %f \n", k.second.code, k.first);
    // }


    while (w.pollEvent(event)) {
        if (event.type == sf::Event::EventType::KeyPressed) {
            int index = indexOfKey(event.key.code);
            if (index == -1) {
                s_keys.push_back(std::make_pair(0, event.key));
                printf("Key: %d pressed\n", event.key.code);
            } else if (s_keys.at(index).first < 0) {
                s_keys.at(index).first = 0;        
                printf("Key: %d pressed\n", event.key.code);

            }
        }
        if (event.type == sf::Event::EventType::KeyReleased) {
            s_keys.at(indexOfKey(event.key.code)).first = -1;
            printf("Key: %d released\n", event.key.code);
        }
        


        if (event.type == sf::Event::Closed) {								//If the event happening is closed: {															//then close the window as well.
            w.close();
        }  
    }

        if (s_buttonTimeout > 0) {
        s_buttonTimeout -= dt;
    }
}

} // Namespace OK