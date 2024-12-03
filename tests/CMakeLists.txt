enable_testing()
add_custom_target(test COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure)

cmake_minimum_required(VERSION 3.10)

project(DuckGameTests)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(testMatch
        match_tests.cpp
        ../server/match.cpp
        ../server/player.cpp
        ../server/receiver.cpp
        ../server/sender.cpp
        ../server/server_protocol.cpp
        ../server/level_manager.cpp
        ../server/game/round_manager.cpp
        ../server/game/box.cpp
        ../server/game/game_map.cpp
        ../server/game/game.cpp
        ../server/game/duck.cpp
        ../server/game/spawn_place.cpp
        ../server/game/game_queue_monitor.cpp
        ../server/game/item.cpp
        ../server/guns/weapon.cpp
        ../server/guns/bouncing_laser.cpp
        ../server/guns/laser.cpp
        ../server/guns/bullet.cpp
        ../server/guns/grenade_fragment.cpp
        ../server/guns/grenade.cpp
        ../server/guns/projectile.cpp
        ../server/guns/banana.cpp
        ../server/guns/sniper.cpp
        ../server/guns/shotgun.cpp
        ../server/guns/magnum.cpp
        ../server/guns/cowboy_pistol.cpp
        ../server/guns/duel_pistol.cpp
        ../server/guns/ak_47.cpp
        ../server/guns/laser_rifle.cpp
        ../server/guns/pew_pew_laser.cpp
        ../server/armor/helmet.cpp
        ../server/armor/armor.cpp
        ../server/executables/guns_commands/shoot_command.cpp
        ../server/executables/guns_commands/drop_weapon_command.cpp
        ../server/executables/guns_commands/take_item_command.cpp
        ../server/executables/start/look_up_command.cpp
        ../server/executables/start/move_down_command.cpp
        ../server/executables/start/move_left_command.cpp
        ../server/executables/start/move_right_command.cpp
        ../server/executables/start/move_up_command.cpp
        ../server/executables/stop/stop_jump_command.cpp
        ../server/executables/stop/stop_look_up_command.cpp
        ../server/executables/stop/stop_move_command.cpp        
        ../server/executables/stop/stop_move_down_command.cpp        
        ../server/executables/stop/stop_move_left_command.cpp        
        ../server/executables/stop/stop_move_right_command.cpp        
        ../common/queue.h
        ../common/liberror.cpp
        ../common/socket.cpp
        ../common/resolver.cpp
        ../common/resolvererror.cpp
)

add_executable(boxTests
        box_tests.cpp
        ../server/match.cpp
        ../server/player.cpp
        ../server/receiver.cpp
        ../server/sender.cpp
        ../server/server_protocol.cpp
        ../server/level_manager.cpp
        ../server/game/round_manager.cpp
        ../server/game/box.cpp
        ../server/game/game_map.cpp
        ../server/game/game.cpp
        ../server/game/duck.cpp
        ../server/game/spawn_place.cpp
        ../server/game/game_queue_monitor.cpp
        ../server/game/item.cpp
        ../server/guns/weapon.cpp
        ../server/guns/bouncing_laser.cpp
        ../server/guns/laser.cpp
        ../server/guns/bullet.cpp
        ../server/guns/grenade_fragment.cpp
        ../server/guns/grenade.cpp
        ../server/guns/projectile.cpp
        ../server/guns/banana.cpp
        ../server/guns/sniper.cpp
        ../server/guns/shotgun.cpp
        ../server/guns/magnum.cpp
        ../server/guns/cowboy_pistol.cpp
        ../server/guns/duel_pistol.cpp
        ../server/guns/ak_47.cpp
        ../server/guns/laser_rifle.cpp
        ../server/guns/pew_pew_laser.cpp
        ../server/armor/helmet.cpp
        ../server/armor/armor.cpp
        ../server/executables/guns_commands/shoot_command.cpp
        ../server/executables/guns_commands/drop_weapon_command.cpp
        ../server/executables/guns_commands/take_item_command.cpp
        ../server/executables/start/look_up_command.cpp
        ../server/executables/start/move_down_command.cpp
        ../server/executables/start/move_left_command.cpp
        ../server/executables/start/move_right_command.cpp
        ../server/executables/start/move_up_command.cpp
        ../server/executables/stop/stop_jump_command.cpp
        ../server/executables/stop/stop_look_up_command.cpp
        ../server/executables/stop/stop_move_command.cpp        
        ../server/executables/stop/stop_move_down_command.cpp        
        ../server/executables/stop/stop_move_left_command.cpp        
        ../server/executables/stop/stop_move_right_command.cpp        
        ../common/queue.h
        ../common/liberror.cpp
        ../common/socket.cpp
        ../common/resolver.cpp
        ../common/resolvererror.cpp
)

add_executable(duckTests
        duck_tests.cpp
        ../server/match.cpp
        ../server/player.cpp
        ../server/receiver.cpp
        ../server/sender.cpp
        ../server/server_protocol.cpp
        ../server/level_manager.cpp
        ../server/game/round_manager.cpp
        ../server/game/box.cpp
        ../server/game/game_map.cpp
        ../server/game/game.cpp
        ../server/game/duck.cpp
        ../server/game/spawn_place.cpp
        ../server/game/game_queue_monitor.cpp
        ../server/game/item.cpp
        ../server/guns/weapon.cpp
        ../server/guns/bouncing_laser.cpp
        ../server/guns/laser.cpp
        ../server/guns/bullet.cpp
        ../server/guns/grenade_fragment.cpp
        ../server/guns/grenade.cpp
        ../server/guns/projectile.cpp
        ../server/guns/banana.cpp
        ../server/guns/sniper.cpp
        ../server/guns/shotgun.cpp
        ../server/guns/magnum.cpp
        ../server/guns/cowboy_pistol.cpp
        ../server/guns/duel_pistol.cpp
        ../server/guns/ak_47.cpp
        ../server/guns/laser_rifle.cpp
        ../server/guns/pew_pew_laser.cpp
        ../server/armor/helmet.cpp
        ../server/armor/armor.cpp
        ../server/executables/guns_commands/shoot_command.cpp
        ../server/executables/guns_commands/drop_weapon_command.cpp
        ../server/executables/guns_commands/take_item_command.cpp
        ../server/executables/start/look_up_command.cpp
        ../server/executables/start/move_down_command.cpp
        ../server/executables/start/move_left_command.cpp
        ../server/executables/start/move_right_command.cpp
        ../server/executables/start/move_up_command.cpp
        ../server/executables/stop/stop_jump_command.cpp
        ../server/executables/stop/stop_look_up_command.cpp
        ../server/executables/stop/stop_move_command.cpp        
        ../server/executables/stop/stop_move_down_command.cpp        
        ../server/executables/stop/stop_move_left_command.cpp        
        ../server/executables/stop/stop_move_right_command.cpp        
        ../common/queue.h
        ../common/liberror.cpp
        ../common/socket.cpp
        ../common/resolver.cpp
        ../common/resolvererror.cpp
)

set(testTargets
        testMatch
        boxTests
        duckTests
)

target_link_libraries(testMatch ${GTEST_LIBRARIES} gtest_main pthread)
target_link_libraries(boxTests ${GTEST_LIBRARIES} gtest_main pthread)
target_link_libraries(duckTests ${GTEST_LIBRARIES} gtest_main pthread)


foreach(target ${testTargets})
		target_compile_definitions(${target} PUBLIC CONFIG_PATH="${CMAKE_SOURCE_DIR}/configurations.yaml")
        add_test(NAME "${target}" COMMAND "${target}")
        add_dependencies(test "${target}")
endforeach()
