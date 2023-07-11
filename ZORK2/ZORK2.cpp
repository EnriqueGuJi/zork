#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

class Location {
public:
    Location(const string& desc) : description(desc) {}

    // Add a bidirectional connection between this location and another location.
    // When adding a connection, also adds the reverse connection from the location to this location.
    void AddConnection(const string& direction, Location* location) {
        connections[direction] = location;
        location->connections[GetOppositeDirection(direction)] = this;
    }

    // Add an item to the location.
    void AddItem(const string& item) {
        items.push_back(item);
    }

    // Check if this location has a specific item.
    bool HasItem(const string& item) const {
        return find(items.begin(), items.end(), item) != items.end();
    }

    // Remove an item from this location.
    void RemoveItem(const string& item) {
        items.erase(remove(items.begin(), items.end(), item), items.end());
    }

    // Function to describe the area in which we are, a description of it and if there is any object
    void Describe() const {
        cout << "Location: " << description << endl;
        cout << "Description: " << GetLocationDescription() << endl;
        cout << "Objects in this location:" << endl;
        for (const string& item : items) {
            cout << "- " << item << endl;
        }
        cout << endl;
    }

    // Get the connected location in the specified direction.
    Location* GetConnection(const string& direction) const {
        if (connections.count(direction) > 0) {
            return connections.at(direction);
        }
        return nullptr;
    }

private:
    string description;
    map<string, Location*> connections;
    vector<string> items;

    // Get the opposite direction to the specified direction.
    static string GetOppositeDirection(const string& direction) {
        if (direction == "north") {
            return "south";
        }
        else if (direction == "south") {
            return "north";
        }
        else if (direction == "east") {
            return "west";
        }
        else if (direction == "west") {
            return "east";
        }
        return "";
    }

    // Function to obtain an improvement of the description of the zone
    string GetLocationDescription() const {
        if (description == "Hallway") {
            return "You find yourself in a dimly lit hallway. The air is filled with a faint musty smell.";
        }
        else if (description == "Garden") {
            return "The sweet scent of blooming roses fills the air as you explore the garden.";
        }
        else if (description == "Dungeon") {
            return "Whispers seem to emanate from the darkness as you cautiously explore the dungeon.";
        }
        else if (description == "Bedroom") {
            return "Soft moonlight seeps through the curtains, casting a gentle glow on the room's wooden furniture.";
        }
        else if (description == "Kitchen") {
            return "Countertops are cluttered with cooking utensils, and pots simmer on the stove.";
        }
        else if (description == "Throne") {
            return "The air is heavy with a sense of power, as if the room itself holds ancient secrets, there is a suspicious candelabrum with green fire...";
        }
        else if (description == "Armory") {
            return "The clinking of armor echoes through the armory as you inspect the various suits and shields.";
        }
        else if (description == "SecretRoom") {
            return "Candlelight casts dancing shadows on the walls, revealing symbols and ancient writings.";
        }
        else if (description == "MinotaurRoom") {
            return "The stench of blood and decay fills the air, warning of the beast's presence.";
        }
        else {
            return "You are in an unknown location.";
        }
    }
};

int main() {

    // Create the game locations.
    Location* hallway = new Location("Hallway" );
    Location* garden = new Location("Garden");
    Location* dungeon = new Location("Dungeon");
    Location* bedroom = new Location("Bedroom");
    Location* kitchen = new Location("Kitchen");
    Location* throne = new Location("Throne");
    Location* armory = new Location("Armory");
    Location* secretRoom = new Location("SecretRoom");
    Location* minotaurRoom = new Location("MinotaurRoom");

    // Set the connections between the locations.
    hallway->AddConnection("south", garden);
    hallway->AddConnection("west", bedroom);
    hallway->AddConnection("east", kitchen);
    hallway->AddConnection("north", throne);

    garden->AddConnection("north", hallway);
    garden->AddConnection("east", dungeon);

    bedroom->AddConnection("east", hallway);
    bedroom->AddConnection("south", secretRoom);

    kitchen->AddConnection("west", hallway);

    throne->AddConnection("south", hallway);
    throne->AddConnection("west", armory);

    armory->AddConnection("east", throne);

    dungeon->AddConnection("west", garden);
    dungeon->AddConnection("east", minotaurRoom);

    // Add items to the desired locations.
    armory->AddItem("Sword");
    secretRoom->AddItem("Key");

    // Set the starting location of the game.
    Location* currentLocation = hallway;

    bool minotaurAlive = true;
    bool secretRoomUnlocked = false;

    bool hasKey = false;
    bool hasSword = false;

    std::string zorkAscii = R"(
 /$$$$$$$$  /$$$$$$  /$$$$$$$  /$$   /$$
|_____ $$  /$$__  $$| $$__  $$| $$  /$$/
     /$$/ | $$  \ $$| $$  \ $$| $$ /$$/ 
    /$$/  | $$  | $$| $$$$$$$/| $$$$$/  
   /$$/   | $$  | $$| $$__  $$| $$  $$  
  /$$/    | $$  | $$| $$  \ $$| $$\  $$ 
 /$$$$$$$$|  $$$$$$/| $$  | $$| $$ \  $$
|________/ \______/ |__/  |__/|__/  \__/                                                                                                                                                                                                                                                                                                                                                                                                                                     
)";

    std::cout << zorkAscii << std::endl;

    cout << "You are in a mysterious mansion, to get out you have to kill the minotaur, good luck adventurer." << endl;
    cout << "Type 'exit' to quit the game." << endl;
    cout << "Type 'help' for available commands." << endl;
    std::cout << "\n";
    std::cout << "\n";
    std::cout << "\n";

    while (true) {

        // Describes where we are in each move.
        currentLocation->Describe();

        // Get the input parameters
        string input;
        cout << "> ";
        getline(cin, input);

        // To exit the game.
        if (input == "exit") {
            break;
        }
        // Help commands for the player.
        else if (input == "help") {
            cout << "Available commands: north, south, east, west, examine, take, drop" << endl;
        }
        // Examine the candelabrum to unlock the secret room.
        else if (input == "examine candelabrum") {
            if (currentLocation == throne) {
                std::cout << "\n";
                cout << "You examine the candelabrum and discover a hidden switch. You activate it." << endl;
                cout << "A slight rumble is heard from the direction of the bedroom." << endl;
                secretRoomUnlocked = true;
            }
            else {
                cout << "There is no candelabrum here to examine." << endl;
            }
        }
        // Move to the specified direction.
        else if (input.substr(0, 6) == "go to " && input.length() > 6) {
            string direction = input.substr(6);
            Location* nextLocation = currentLocation->GetConnection(direction);
            if (nextLocation != nullptr) {
                // Check if the secret room is activated and report the player.
                if (nextLocation == secretRoom) {
                    if (secretRoomUnlocked) {
                        currentLocation = nextLocation;
                    }
                    else {
                        std::cout << "\n";
                        cout << "You feel the wind coming out of a hole in the wall." << endl;
                    }
                }
                // Check if the player has the key or not to enter the dungeon.
                else if (nextLocation == dungeon) {
                    if (hasKey) {
                        std::cout << "\n";
                        cout << "You have unlocked the dungeon door with the key." << endl;
                        currentLocation = nextLocation;
                    }
                    else {
                        std::cout << "\n";
                        cout << "The dungeon is locked. You need a key to enter." << endl;
                    }
                }
                // Check if the player has the sword to kill the minotaur, and if not, the player loses.
                else if (nextLocation == minotaurRoom) {
                    if (hasSword) {
                        if (minotaurAlive) {
                            std::cout << "\n";
                            cout << "You enter the Minotaur's room and confront the beast!" << endl;
                            cout << "You swing your sword with all your might and slay the Minotaur!" << endl;
                            minotaurAlive = false;

                            std::string zorkAscii = R"(
  /$$$$$$                                                     /$$              /$$
 /$$__  $$                                                   | $$             | $$
| $$  \__/  /$$$$$$  /$$$$$$$   /$$$$$$   /$$$$$$  /$$$$$$  /$$$$$$   /$$$$$$$| $$
| $$       /$$__  $$| $$__  $$ /$$__  $$ /$$__  $$|____  $$|_  $$_/  /$$_____/| $$
| $$      | $$  \ $$| $$  \ $$| $$  \ $$| $$  \__/ /$$$$$$$  | $$   |  $$$$$$ |__/
| $$    $$| $$  | $$| $$  | $$| $$  | $$| $$      /$$__  $$  | $$ /$$\____  $$    
|  $$$$$$/|  $$$$$$/| $$  | $$|  $$$$$$$| $$     |  $$$$$$$  |  $$$$//$$$$$$$/ /$$
 \______/  \______/ |__/  |__/ \____  $$|__/      \_______/   \___/ |_______/ |__/
                               /$$  \ $$                                          
                              |  $$$$$$/                                          
                               \______/ 
)";
                            std::cout << zorkAscii << std::endl;
                            break;
                        }                     
                    }
                    else {
                        std::cout << "\n";
                        cout << "You enter the Minotaur's room, but you are helpless. The Minotaur knocks you down and skewers you with his horns, you die." << endl;

                        std::string zorkAscii = R"(
  /$$$$$$                                           /$$$$$$                               
 /$$__  $$                                         /$$__  $$                              
| $$  \__/  /$$$$$$  /$$$$$$/$$$$   /$$$$$$       | $$  \ $$ /$$    /$$ /$$$$$$   /$$$$$$ 
| $$ /$$$$ |____  $$| $$_  $$_  $$ /$$__  $$      | $$  | $$|  $$  /$$//$$__  $$ /$$__  $$
| $$|_  $$  /$$$$$$$| $$ \ $$ \ $$| $$$$$$$$      | $$  | $$ \  $$/$$/| $$$$$$$$| $$  \__/
| $$  \ $$ /$$__  $$| $$ | $$ | $$| $$_____/      | $$  | $$  \  $$$/ | $$_____/| $$      
|  $$$$$$/|  $$$$$$$| $$ | $$ | $$|  $$$$$$$      |  $$$$$$/   \  $/  |  $$$$$$$| $$      
 \______/  \_______/|__/ |__/ |__/ \_______/       \______/     \_/    \_______/|__/ 
)";
                        std::cout << zorkAscii << std::endl;
                        break;
                    }
                }
                else {
                    currentLocation = nextLocation;
                }
            }
            // If there is no connection in the specified direction, notify that it's not possible to go that way.
            else {
                cout << "You can't go in that direction." << endl;
            }
        }
        // Take command to pick up the selected item.
        else if (input.substr(0, 5) == "take " && input.length() > 5) {
            string item = input.substr(5);
            if (currentLocation->HasItem(item)) {
                if (item == "Key") {
                    if (!hasKey) {
                        currentLocation->RemoveItem(item);
                        std::cout << "\n";
                        cout << "You have picked up the " << item << "." << endl;
                        hasKey = true;              
                    }
                    else {
                        cout << "You already have the key in your inventory." << endl;
                    }
                }
                else if (item == "Sword") {
                    if (!hasSword) {
                        currentLocation->RemoveItem(item);
                        std::cout << "\n";
                        cout << "You have picked up the " << item << "." << endl;
                        hasSword = true;
                    }
                    else {
                        cout << "You already have the sword in your inventory." << endl;
                    }
                }
                else {
                    cout << "You cannot take that item." << endl;
                }
            }
            else {
                cout << "There is no " << item << " here to take." << endl;
            }
        }
        // Drop command to remove the selected item from the player's inventory and place it in the current location.
        else if (input.substr(0, 5) == "drop " && input.length() > 5) {
            string item = input.substr(5);
            if (item == "Key") {
                if (hasKey) {
                    currentLocation->AddItem(item);
                    std::cout << "\n";
                    cout << "You have dropped the " << item << "." << endl;
                    hasKey = false;
                }
                else {
                    cout << "You don't have the key in your inventory." << endl;
                }
            }
            else if (item == "Sword") {
                if (hasSword) {
                    currentLocation->AddItem(item);
                    std::cout << "\n";
                    cout << "You have dropped the " << item << "." << endl;
                    hasSword = false;
                }
                else {
                    cout << "You don't have the sword in your inventory." << endl;
                }
            }
            else {
                cout << "You cannot drop that item." << endl;
            }
        }
        else {
            cout << "Invalid command. Type 'help' for available commands." << endl;
        }
        cout << endl;
    }
    cout << "Press any key to exit...";
    cin.get();
    return 0;
}