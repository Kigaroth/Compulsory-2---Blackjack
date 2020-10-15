// Compulsory 2 - Blackjack.cpp //

#include <iostream>
#include <vector>
#include <conio.h>
#include <ctime>

bool player_hold = false;
bool dealer_hold = false;
bool choice = false;
bool player_round_over = false;
bool dealer_round_over = false;
bool game_over = false;

int player_value = 0;
int dealer_value = 0;
int round_win = 0;
int player_cardcount = 0;
int dealer_cardcount = 0;
int player_bet = 0;

int dealer_money = 100;
int player_money = 90;
int money_pot = 10;

const int round_cost = 10;
const int bj_max_score = 21;

std::vector <int> player_hand = {};
std::vector <int> dealer_hand = {};

//functions
int generate_card();
int calculate_player_hand();
int calculate_dealer_hand();
void betting_round();
void calculate_moneytotals();
void start_new_round();
void check_maxscore_or_loss();
void show_current_wallets(bool showpot);
void player_draw_card();
void player_turn();
void dealer_turn();
void print_winner();
void set_game_over();
void new_round();
void reset_game();
void start_screen();
void rules();

int main()
{
    srand(static_cast<unsigned int>(time(nullptr))); //generate a random seed for generating the card values.
    
    start_screen();
    rules();

    do {
        //Display player and Dealer money
        show_current_wallets(true); //when set to true, the current pot is shown.
        //Player turn
        do {
            player_turn();
            player_value = calculate_player_hand();
            std::cout << "\n You currently have a hand with a total value of " << player_value << std::endl;
            check_maxscore_or_loss();
        } while (!player_round_over);
       
        //Dealer turn
        if (player_value <= bj_max_score){ //Dealers turn only goes into effect if the players hand hasn't exceeded a total value of 21.
            betting_round();
            do {
                dealer_turn();
                dealer_value = calculate_dealer_hand();
                check_maxscore_or_loss();
            } while (!dealer_round_over);
            std::cout << "\n The dealer currently have a hand with a total value of " << dealer_value << std::endl;
        }

        //Print the winner of the round and adds/subtracts winnings/loss.
        print_winner();
        calculate_moneytotals();
        show_current_wallets(false); //false to not show the moneypot. The moneypot isn't reset until the player chooses to buy into a new round
        set_game_over(); //function that sets the game to be over if the player or dealer runs out of money.

        //Give the user a choice to start a new round if they want, or end the game. If the player has won or lost all the money, this step is skipped.
        if (!game_over) {
            new_round(); //function resets all variables and vectors needed in order to start a new round.
        }

    } while (!game_over);

    system("PAUSE");
}

int generate_card() {
    int cardvalue = rand() % 13 + 1;
    return cardvalue;
}

void player_draw_card() {
    int card = generate_card();

    if (card == 1) {
        int choice_ace = 0;
        do {
            std::cout << R"(
        .------.
        |A.--. |
        | :/\: |
        | (__) |
        | '--'A|
        `------')";
            std::cout << "\n You drew an Ace. Would you like it to have a value of 1 or 11? \n\t1. Value of 1. \n\t2. Value of 11. \n Enter your choice here: ";
            std::cin >> choice_ace;
        } while (choice_ace == 0 || choice_ace > 2);
        if (choice_ace == 1) {
            card = 1;
        }
        else if (choice_ace == 2) {
            card = 11;
        }
    }
    else if (card == 11) {
        std::cout << R"(
        .------.
        |J.--. |
        | :/\: |
        | (__) |
        | '--'J|
        `------')";
        std::cout << "\n You drew a Jack. It has a value of 10." << std::endl;
        card = 10;
    }
    else if (card == 12) {
        std::cout << R"(
        .------.
        |Q.--. |
        | :/\: |
        | (__) |
        | '--'Q|
        `------')";
        std::cout << "\n You drew a Queen. It has a value of 10." << std::endl;
        card = 10;
    }
    else if (card == 13) {
        std::cout << R"(
        .------.
        |K.--. |
        | :/\: |
        | (__) |
        | '--'K|
        `------')";
        std::cout << "\n You drew a King. It has a value of 10." << std::endl;
        card = 10;
    }
    else{
        std::cout << "\n You drew a: " << R"(
        .------.
        |)" << card << R"(.--. |
        | :/\: |
        | (__) |
        | '--')" << card << R"(|
        `------')" << std::endl;
    }
    player_hand.push_back(card);
    for (int ix = 0; ix < player_cardcount; ix++) {
        std::cout << player_hand[ix] << ", ";
    }
}

void dealer_draw_card() {
    int card = generate_card();

    if (card == 1) {
        int choice_ace = 0;
        do {
            std::cout << "\n The Dealer drew an Ace. " << std::endl;
            if (dealer_value <= 10) {
                card = 11;
                std::cout << " The Dealer chose to count it as an 11." << std::endl;
                choice_ace = 1;
            }
            if (dealer_value > 10){
                card = 1;
                std::cout << " The Dealer chose to count it as a 1." << std::endl;
                choice_ace = 2;
            }
        } while (choice_ace == 0 || choice_ace > 2);
    }

    else if (card == 11) {
        std::cout << R"(
        .------.
        |J.--. |
        | :/\: |
        | (__) |
        | '--'J|
        `------')";
        std::cout << "\n The Dealer drew a Jack. It has a value of 10." << std::endl;
        card = 10;
    }
    else if (card == 12) {
        std::cout << R"(
        .------.
        |Q.--. |
        | :/\: |
        | (__) |
        | '--'Q|
        `------')";
        std::cout << "\n The Dealer drew a Queen. It has a value of 10." << std::endl;
        card = 10;
    }
    else if (card == 13) {
        std::cout << R"(
        .------.
        |K.--. |
        | :/\: |
        | (__) |
        | '--'K|
        `------')";
        std::cout << "\n The Dealer drew a King. It has a value of 10." << std::endl;
        card = 10;
    }
    else{
        std::cout << "\n The dealer drew a: " << R"(
        .------.
        |)" << card << R"(.--. |
        | :/\: |
        | (__) |
        | '--')" << card << R"(|
        `------')" << std::endl;
    }
    dealer_hand.push_back(card);
    dealer_cardcount++;
}

void dealer_turn() {
    if (dealer_value < 18) {
        dealer_draw_card();
        std::cout << "\n The dealer currently have these cards in his hand: ";
        for (int ix = 0; ix < dealer_cardcount; ix++) {
            std::cout << dealer_hand[ix] << ", ";
        }
    }
    else {
        dealer_hold = true;
    }
    if (dealer_hold == true) {
        dealer_round_over = true;
    }
}

void player_turn(){
    std::cout << "\n Would you like to draw a new card (Press 'D') or would you like to hold your hand (Press 'H')?    ";

    char player_input = _getch();

    do {
        switch (player_input) {

        case 'd':
        case 'D':
            std::cout << player_input;
            choice = true;
            player_cardcount++;
            player_draw_card();
            std::cout << "\n You have drawn " << player_cardcount << " cards." << std::endl;
            break;

        case 'h':
        case 'H':
            std::cout << player_input;
            choice = true;
            player_hold = true;

            break;
        }
    } while (choice == false);
}

void betting_round() {
    std::cout << "\n Would you like to make a bet? Press 'B' to make a bet, Press 'S' to skip betting." << std::endl;


    int bet_amount = 0;
    bool bet_valid = false;
    do{
        char bet_choice = _getch();
        switch (bet_choice) {
        case 'b':
        case 'B':
            do {
                std::cout << "\n You currently have a total of: " << player_money << "$." << std::endl;
                std::cout << " The Dealer currenlty have a total of: " << dealer_money << "$." << std::endl;
                std::cout << "\n How much money would you like to bet ?\n Type amount here:";
                std::cin >> bet_amount;
                if (bet_amount > player_money || bet_amount > dealer_money) {
                    std::cout << "\n Invalid amount. Bet can't exceed yours or the Dealers current amount of money. Try again!" << std::endl;
                }
                else {
                    bet_valid = true;
                }
            } while (!bet_valid);
            player_bet = bet_amount;
            break;

        case 's':
        case 'S':
            bet_valid = true;
            break;
        default:
            break;

        }
    } while (!bet_valid);

}

int calculate_player_hand() {
    int val = 0;
    for (int ix = 0; ix < player_hand.size(); ix++) {
        val += player_hand[ix];
    }
    return val;
}

int calculate_dealer_hand() {
    int val = 0;
    for (int ix = 0; ix < dealer_hand.size(); ix++) {
        val += dealer_hand[ix];
    }
    return val;
}

void check_maxscore_or_loss() {
    if (player_value > bj_max_score) {
        player_round_over = true;
    }
    if (player_value == bj_max_score) {
        player_round_over = true;
    }
    if (player_hold == true) {
        player_round_over = true;
    }
    if (dealer_value > bj_max_score) {
        dealer_round_over = true;
    }
    if (dealer_value == bj_max_score) {
        dealer_round_over = true;
    }
}

void show_current_wallets(bool showpot) {
    std::cout << "\t\tThe player currently has " << player_money << "$. " << std::endl;
    std::cout << "\t\tThe Dealer currently has " << dealer_money << "$. " << std::endl;
    if (showpot) {
        std::cout << "\t\tThe pot currently has " << money_pot << "$ in it." << std::endl;
    }

}

void print_winner() {
    if (player_value > bj_max_score) {
        std::cout << "\n The Dealer has won! \n" << std::endl;
        round_win = 1;
    }
    else if (dealer_value == 21 && player_value < 21) {
        std::cout << "\n The Dealer has won! \n" << std::endl;
        round_win = 1;

    }
    else if (dealer_value == 21 && player_value < 21) {
        std::cout << "\n The Dealer has won! \n" << std::endl;
        round_win = 1;
    }
    else if (dealer_value > player_value && dealer_value < bj_max_score) {
        std::cout << "\n The Dealer has won! \n" << std::endl;
        round_win = 1;
    }
    else if (dealer_value > bj_max_score)
    {
        std::cout << "\n The player has won! \n" << std::endl;
        round_win = 2;
    }
    else if (player_value > dealer_value) {
        std::cout << "\n The player has won! \n" << std::endl;
        round_win = 2;
    }
    else if (player_value == dealer_value) {
        std::cout << "\n The game has reached a draw. \n" << std::endl;
        round_win = 3;
    }
}

void new_round() {
    std::cout << "\n\n Do you want to start a new round? (Cost of entry is 10$) \n\t1. Yes\n\t2. No";

    bool choicevalid = false;

    do {
    char restartroundchoice = _getch();

        switch (restartroundchoice){
        case '1':
            start_new_round();
            choicevalid = true;
            break;
        case '2':
            game_over = true;
            choicevalid = true;
            std::cout << "\n\n";
            break;

        default:
            break;
        }
    } while (!choicevalid);
}

void calculate_moneytotals() {

    if (round_win == 1) {
        money_pot += player_bet;
        dealer_money += money_pot;
        player_money -= player_bet;
    }
    else if (round_win == 2) {
        money_pot += player_bet;
        dealer_money -= player_bet;
        player_money += money_pot;
    }
    else {
        money_pot += player_bet*2;
        dealer_money -= player_bet;
        player_money -= player_bet;
    }

}

void set_game_over() {
    if (player_money == 0) {
        std::cout << " You've lost all your money. Game over :(" << std::endl;
        game_over = true;
    }
    else if (dealer_money == 0) {
        std::cout << " You've won all of the Dealers money! Congratulations! :D" << std::endl;
        game_over = true;
    }
}

void reset_game() {

// Values that have to be reset in order for the round to start fresh.
    player_hold = false;
    dealer_hold = false;
    choice = false;
    player_round_over = false;
    dealer_round_over = false;
    player_cardcount = 0;
    dealer_cardcount = 0;
    player_bet = 0;
    player_value = 0;
    dealer_value = 0;

    if (round_win == 1 || round_win == 2) { //Resets the money pot. If it ends at a draw (round_win = 3), the pot carries over to the next round.
        money_pot = 0;
    }
    round_win = 0;

    player_hand.clear();
    dealer_hand.clear();

    system("CLS");

}

void start_new_round() {
    reset_game(); //uses the reset_game function to reset all variables (except the player and dealers wallet) changed during the game.
    player_money -= round_cost; //The cost to start a new round is subtracted from the player
    money_pot += round_cost; //The cost the player paid is added to the pot.
}

void start_screen(){

    std::cout << "\t\t" << R"(
                .______    __          ___       ______  __  ___        __       ___       ______  __  ___ 
                |   _  \  |  |        /   \     /      ||  |/  /       |  |     /   \     /      ||  |/  / 
                |  |_)  | |  |       /  ^  \   |  ,----'|  '  /        |  |    /  ^  \   |  ,----'|  '  /  
                |   _  <  |  |      /  /_\  \  |  |     |    <   .--.  |  |   /  /_\  \  |  |     |    <   
                |  |_)  | |  `----./  _____  \ |  `----.|  .  \  |  `--'  |  /  _____  \ |  `----.|  .  \  
                |______/  |_______/__/     \__\ \______||__|\__\  \______/  /__/     \__\ \______||__|\__\ 
                                                                                           
)" << std::endl;

    system("PAUSE");
    system("CLS");


}

void rules(){

    std::cout << "\t" << R"(
                .______    __          ___       ______  __  ___        __       ___       ______  __  ___ 
                |   _  \  |  |        /   \     /      ||  |/  /       |  |     /   \     /      ||  |/  / 
                |  |_)  | |  |       /  ^  \   |  ,----'|  '  /        |  |    /  ^  \   |  ,----'|  '  /  
                |   _  <  |  |      /  /_\  \  |  |     |    <   .--.  |  |   /  /_\  \  |  |     |    <   
                |  |_)  | |  `----./  _____  \ |  `----.|  .  \  |  `--'  |  /  _____  \ |  `----.|  .  \  
                |______/  |_______/__/     \__\ \______||__|\__\  \______/  /__/     \__\ \______||__|\__\                                                                      
    )" << std::endl;

    std::cout << R"(

                                                       RULES
    __________________________________________________________________________________________________________________
    |                                                                                                                |
    | - The player starts with 100$. Each round costs 10$. The Dealer also starts with 100$                          |
    | - The player draws a card with 'D'.                                                                            |
    | - The player can draw as many cards as they want, but the total value of their hand can't go above 21.         | 
    | - The player can chose to hold their hand after a card is drawn by pressing 'H'                                |
    | - The player can bet on their hand of cards after it's being held.                                             |
    | - The dealers turn starts after the betting round.                                                             |
    | - If the dealer can't match the players score, or their hand of cards has a value above 21, the player wins.   |
    | - If the Dealers hand of cards has a value that's higher than the players, the Dealer wins.                    |
    | - When the round is over, the game will calculate the new amounts of money the player and the Dealer has.      |
    | - The player can buy into a new round, or choose to exit the game after this point.                            |
    | - The game ends automatically if the player or the Dealer runs out of money                                    |
    |________________________________________________________________________________________________________________|
                                                                                    
    )" << std::endl;

system("PAUSE");
system("CLS");

}