#include "duck.h"
#include <iostream>
#include <typeinfo>

const int HELMET_BROKE = 0;
const int ARMOR_BROKE = 1;
const int DEAD = 2;

Duck::Duck(char id, int x, int y, GameMap* map) :
    id_player(id),
    position(x, y),
    old_position(x, y),
    map(map),
    is_moving(false),
    speed_x(0),
    speed_y(0),
    looking(LOOKING_RIGHT),
    is_jumping(false),
    is_fluttering(false),
    is_laying_down(false),
    was_laying_down(false),
    is_slippy(false),
    is_looking_up(false),
    was_looking_up(false),
    life_points(1),
    stop_notificated(false),
    is_dead(false),
    weapon(nullptr),
    armor(nullptr), //suma 1 a lifepoint
    helmet(nullptr), //suma 1 a lifepoint
    onHand(nullptr) {}

bool Duck::is_in_air(){
    return map->canMoveDuckTo(position.x, position.y + 1, id_player);
}

bool Duck::next_to_wall(){
    return (!map->canMoveDuckTo(position.x + 1, position.y, id_player) || !map->canMoveDuckTo(position.x - 1, position.y, id_player));
}

bool Duck::pickUpItem(std::shared_ptr<Item> item) {
    if (item != nullptr){
        std::cout << "Agarro el item de ID " << item->getItemId() << "\n";
        
        if (onHand == nullptr)
            std::cout << "No tiene un ítem en la mano actualmente" << "\n";
        else
            std::cout << "El item de la mano antes de cambiarlo tiene ID " << onHand->getItemId() << "\n";
        
        onHand = item;
        std::cout << "Ahora el Id del Item en la mano es " << onHand->getItemId() << "\n";
        return true;
    } 
    
    std::cout << "No hay nada para agarrar aca!" << "\n";
    return false;

}

void Duck::useOnHand() {
    if (!onHand) return;  // Verificamos si hay un item en la mano

    if (auto w = std::dynamic_pointer_cast<Weapon>(onHand)) {
        setWeapon(w);
    } else if (auto a = std::dynamic_pointer_cast<Armor>(onHand)) {
        setArmor(a);
    } else if (auto h = std::dynamic_pointer_cast<Helmet>(onHand)) {
        setHelmet(h);
    }

    onHand.reset();  // Libera el recurso
}

void Duck::check_gravity(){

    if(is_in_air()) {
        if (is_fluttering){
            //si esta aleteando cae a velocidad constante de 1
            speed_y = DUCK_FALL_POWER;
        }else {
            //sino, cae con aceleracion
            speed_y += DUCK_FALL_POWER;
        }
        
    }

}

int Duck::update_life(){
    //Esta funcion tiene que avisar que le sacaron el casco, le sacaron el armor, o que murio

    if(map->duckIsOverVoid(position.x, position.y)){
        is_dead = true;
    }


    if(map->duckIsOverBullet(position)){

        if (helmet != nullptr){ //si tengo helmet me saca el helmet
            helmet = nullptr;
            life_points -= 1;
            return HELMET_BROKE;
        }

        if (armor != nullptr){ //si tengo armor me saca el armor
            armor = nullptr;
            life_points -= 1;
            return ARMOR_BROKE;

        }


        life_points -= 1;
        if (life_points == 0){
            is_dead = true;
        }
    }

    if (is_dead) {
        map->cleanDuckOldPosition(position.x, position.y);
    }

    return DEAD;

}

void Duck::update_position() {
    if(is_dead){return;}

    check_gravity();
    
    int delta_x = position.x;
    int delta_y = position.y;

    if (is_slippy) {
        int slippery_vec = (looking == LOOKING_RIGHT) ? 1 : -1;
        delta_x += 4*slippery_vec;
        delta_y += speed_y;
    } else {
        delta_x += speed_x;
        delta_y += speed_y;
    }

    Position new_pos(delta_x, delta_y);
    old_position = position;

    Position banana_position {0, 0};
    if (map->duckIsOverBanana(new_pos, banana_position)) {
        is_slippy = true;
    }

    //mueve al pato a la nueva posicion si esta libre o a la que este libre inmediatamente antes
    position = map->move_duck_to(position, new_pos, id_player);


    // Ver si tiene una pared al lado
    if (next_to_wall()) {
        is_slippy = false;
    }

    if ((is_jumping || is_fluttering) && !is_in_air()){
        //si esta saltando o aleteando pero no esta en el aire, significa que aterrizo
        is_jumping = false;
        is_fluttering = false;
        speed_y = 0;
    }

    if(!is_in_air()){
        speed_y = 0;
    }

}


void Duck::update_weapon(){
    
    if (weapon != nullptr) {
        if (weapon->getItemId() == GRENADE_ID) {
            Grenade* grenade = (Grenade*) weapon.get();
            if (grenade->update_weapon(position.x, position.y, looking, map, id_player))
                weapon.reset();
        } else {
            weapon->update_weapon();
        }
    }
}

bool Duck::get_duck_dead_message(Message& msg){
    if (!is_dead){
        return false;
    }

    msg.type = KILL_DUCK;
    msg.player_id = static_cast<uint16_t>(id_player - '0');
    return true;
}

bool Duck::get_duck_broke_helmet_message(Message& msg){

    msg.type = HELMET_BROKEN;
    msg.player_id = static_cast<uint16_t>(id_player - '0');
    return true;
}

bool Duck::get_duck_broke_armor_message(Message& msg){

    msg.type = ARMOR_BROKEN;
    msg.player_id = static_cast<uint16_t>(id_player - '0');
    return true;
}

void Duck::form_position_message(Message& msg){
    msg.type = DUCK_POS_UPDATE;
    msg.player_id = static_cast<uint16_t>(id_player - '0'); // Convertimos el id a int
    msg.duck_x = position.x;
    msg.duck_y = position.y;
    msg.looking = looking;
    msg.is_moving = is_moving;
    msg.is_jumping = is_jumping;
    msg.is_fluttering = is_fluttering;
    msg.is_laying_down = is_laying_down;
    msg.is_looking_up = is_looking_up;
}

bool Duck::get_duck_position_message(Message& msg){

    if(is_dead){return false;}

    if(is_looking_up != was_looking_up){
        form_position_message(msg);
        was_looking_up = is_looking_up;
        return true;
    }

    if (is_laying_down != was_laying_down) {

        form_position_message(msg);
        was_laying_down = is_laying_down;
        return true;
    }

    if (old_position.x == position.x && old_position.y == position.y){
        if(stop_notificated){
            return false;
        } else {

            form_position_message(msg);
            stop_notificated = true;

            return true;
        }
    }

    form_position_message(msg);
    stop_notificated = false;

    return true;
}


bool Duck::get_duck_initialize_message(Message& msg){
    form_position_message(msg);
    return true;
}

char Duck::get_id() const {
    return id_player;
}

void Duck::setWeapon(std::shared_ptr<Weapon> new_weapon) {
    std::cout << "ASIGNO NUEVA ARMA" << "\n";

    switch (new_weapon->getItemId()) {
        case BANANA_ID:
            weapon = std::make_shared<Banana>();
            break;
        case GRENADE_ID:
            weapon = std::make_shared<Grenade>();
            break;
        case COWBOY_PISTOL_ID:
            weapon = std::make_shared<CowboyPistol>();
            break;
        case PEW_PEW_LASER_ID:
            weapon = std::make_shared<PewPewLaser>();
            break;
        case LASER_RIFLE_ID:
            weapon = std::make_shared<LaserRifle>();
            break;
        case AK_47_ID:
            weapon = std::make_shared<Ak47>();
            break;
        case DUEL_PISTOL_ID:
            weapon = std::make_shared<DuelPistol>();
            break;
        case MAGNUM_ID:
            weapon = std::make_shared<Magnum>();
            break;
        case SHOTGUN_ID:
            weapon = std::make_shared<Shotgun>();
            break;
        case SNIPER_ID:
            weapon = std::make_shared<Sniper>();
            break;
        default:
            std::cout << "Tipo de arma desconocido" << "\n";
    }
}

void Duck::setArmor(std::shared_ptr<Armor> new_armor) {
    std::cout << "ASIGNO NUEVA ARMADURA" << "\n";
    if (!armor) {
        armor = new_armor;
        life_points += 1; 
    } else {
        std::cout << "El pato ya tiene una armadura equipada" << "\n";
    }
}

void Duck::setHelmet(std::shared_ptr<Helmet> new_helmet) {
    if (!helmet) {
        std::cout << "ASIGNO NUEVO CASCO" << "\n";
        helmet = new_helmet;
        life_points += 1;
    } else {
        std::cout << "El pato ya tiene un casco equipado" << "\n";
    }
}

uint8_t Duck::getWeaponId(){
    if (weapon != nullptr){
        return weapon->getItemId();
    }
    return 0;
}


bool Duck::disparar() {
    if(is_dead){return false;}

    if (weapon != nullptr) {
        bool habia_municiones = (weapon->getMuniciones() > 0);
        if(!(weapon->disparar(position.x, position.y, looking, map, id_player, is_looking_up))){
            //si no pude disparar devuevlo false
            return false;
        }

        
        //RETROCESOOO PARA AK47 Y MAGNUM
        if (habia_municiones && (weapon->getItemId() == AK_47_ID || weapon->getItemId() == MAGNUM_ID)) {
            old_position = position;

            int shoot_speed = (looking == LOOKING_RIGHT) ? -1 : 1;
            int delta_x = position.x + shoot_speed;
            int delta_y = position.y;

            Position new_pos(delta_x, delta_y);
            
            //mueve al pato a la nueva posicion si esta libre o a la que este libre inmediatamente antes
            position = map->move_duck_to(position, new_pos, id_player);
        }

        if (habia_municiones){
            return true;
        }

    }
    return false;
}


Position Duck::getPosition(){
    return position;
}

void Duck::reset_for_round(Position pos){
    position = pos;
    old_position = pos;
    is_moving = false;
    speed_x = 0;
    speed_y = 0;
    looking = LOOKING_RIGHT;
    is_jumping = false;
    is_fluttering= false;
    is_laying_down= false;
    was_laying_down= false;
    is_slippy= false;
    life_points = 1;
    stop_notificated = false;
    is_dead = false;
    weapon = nullptr;
    armor = nullptr;
    helmet = nullptr;
    onHand.reset();
}

std::shared_ptr<Item> Duck::getItemOnHand() const {
    return onHand ? onHand : nullptr;
}

std::shared_ptr<Weapon> Duck::dropWeapon() {
    if (weapon) {
        std::cout << "El jugador " << id_player << " deja caer su arma: " << std::endl;

        if (weapon->getItemId() == GRENADE_ID) {
            Grenade* weapon_grenade = dynamic_cast<Grenade*>(weapon.get());
            weapon_grenade->prepare_drop(looking);
            weapon_grenade->setPosition(position.x, position.y);
        } else if (weapon->getItemId() == BANANA_ID) {
            Banana* weapon_banana = dynamic_cast<Banana*>(weapon.get());
            weapon_banana->prepare_drop(looking);
            weapon_banana->setPosition(position.x, position.y);
        }

        return std::move(weapon);
    }

    return nullptr;
}



