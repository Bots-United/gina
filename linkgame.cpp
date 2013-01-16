/*
 * The GINA Bot - a computer opponent for Valve's FPS game Half-Life
 * Copyright (c) 2011, Wei Mingzhi <whistler_wmz@users.sf.net>
 *
 * This file is part of The GINA Bot.
 *
 * The GINA Bot is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * The GINA Bot is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with The GINA Bot; if not, visit <http://www.gnu.org/licenses>.
 *
 * In addition, as a special exception, the author gives permission to
 * link the code of this program with the Half-Life Game Engine ("HL
 * Engine") and Modified Game Libraries ("MODs") developed by Valve,
 * L.L.C ("Valve").  You must obey the GNU General Public License in all
 * respects for all of the code used other than the HL Engine and MODs
 * from Valve.  If you modify this file, you may extend this exception
 * to your version of the file, but you are not obligated to do so.  If
 * you do not wish to do so, delete this exception statement from your
 * version.
 */

//
// linkgame.cpp
//
// Export entities from mod DLL back to the HL engine
//
// Based on: HPB bot - botman's High Ping Bastard bot
// (http://planethalflife.com/botman/)
//

#include "main.h"

// Function to perform common code of LINK_ENTITY_TO_GAME.
void LinkEnt(LINK_ENTITY_GAME *pfnEnt, char *entname, entvars_t *pev)
{
   if (!*pfnEnt)
      *pfnEnt = (LINK_ENTITY_GAME)dlsym(h_Library, entname);

   if (!*pfnEnt)
      return;

   (*pfnEnt)(pev);
}

#define LINK_ENTITY_TO_GAME(mapClassName)           \
C_DLLEXPORT void mapClassName(entvars_t *pev)       \
{                                                   \
   static LINK_ENTITY_GAME otherClassName = NULL;   \
   LinkEnt(&otherClassName, #mapClassName, pev);    \
}

// entities for Valve's hl.dll and Standard SDK...
LINK_ENTITY_TO_GAME(DelayedUse);
LINK_ENTITY_TO_GAME(aiscripted_sequence);
LINK_ENTITY_TO_GAME(ambient_generic);
LINK_ENTITY_TO_GAME(ammo_357);
LINK_ENTITY_TO_GAME(ammo_9mmAR);
LINK_ENTITY_TO_GAME(ammo_9mmbox);
LINK_ENTITY_TO_GAME(ammo_9mmclip);
LINK_ENTITY_TO_GAME(ammo_ARgrenades);
LINK_ENTITY_TO_GAME(ammo_buckshot);
LINK_ENTITY_TO_GAME(ammo_crossbow);
LINK_ENTITY_TO_GAME(ammo_egonclip);
LINK_ENTITY_TO_GAME(ammo_gaussclip);
LINK_ENTITY_TO_GAME(ammo_glockclip);
LINK_ENTITY_TO_GAME(ammo_mp5clip);
LINK_ENTITY_TO_GAME(ammo_mp5grenades);
LINK_ENTITY_TO_GAME(ammo_rpgclip);
LINK_ENTITY_TO_GAME(beam);
LINK_ENTITY_TO_GAME(bmortar);
LINK_ENTITY_TO_GAME(bodyque);
LINK_ENTITY_TO_GAME(button_target);
LINK_ENTITY_TO_GAME(cine_blood);
LINK_ENTITY_TO_GAME(controller_energy_ball);
LINK_ENTITY_TO_GAME(controller_head_ball);
LINK_ENTITY_TO_GAME(crossbow_bolt);
LINK_ENTITY_TO_GAME(cycler);
LINK_ENTITY_TO_GAME(cycler_prdroid);
LINK_ENTITY_TO_GAME(cycler_sprite);
LINK_ENTITY_TO_GAME(cycler_weapon);
LINK_ENTITY_TO_GAME(cycler_wreckage);
LINK_ENTITY_TO_GAME(env_beam);
LINK_ENTITY_TO_GAME(env_beverage);
LINK_ENTITY_TO_GAME(env_blood);
LINK_ENTITY_TO_GAME(env_bubbles);
LINK_ENTITY_TO_GAME(env_debris);
LINK_ENTITY_TO_GAME(env_explosion);
LINK_ENTITY_TO_GAME(env_fade);
LINK_ENTITY_TO_GAME(env_funnel);
LINK_ENTITY_TO_GAME(env_global);
LINK_ENTITY_TO_GAME(env_glow);
LINK_ENTITY_TO_GAME(env_laser);
LINK_ENTITY_TO_GAME(env_lightning);
LINK_ENTITY_TO_GAME(env_message);
LINK_ENTITY_TO_GAME(env_render);
LINK_ENTITY_TO_GAME(env_shake);
LINK_ENTITY_TO_GAME(env_shooter);
LINK_ENTITY_TO_GAME(env_smoker);
LINK_ENTITY_TO_GAME(env_sound);
LINK_ENTITY_TO_GAME(env_spark);
LINK_ENTITY_TO_GAME(env_sprite);
LINK_ENTITY_TO_GAME(fireanddie);
LINK_ENTITY_TO_GAME(func_breakable);
LINK_ENTITY_TO_GAME(func_button);
LINK_ENTITY_TO_GAME(func_conveyor);
LINK_ENTITY_TO_GAME(func_door);
LINK_ENTITY_TO_GAME(func_door_rotating);
LINK_ENTITY_TO_GAME(func_friction);
LINK_ENTITY_TO_GAME(func_guntarget);
LINK_ENTITY_TO_GAME(func_healthcharger);
LINK_ENTITY_TO_GAME(func_illusionary);
LINK_ENTITY_TO_GAME(func_ladder);
LINK_ENTITY_TO_GAME(func_monsterclip);
LINK_ENTITY_TO_GAME(func_mortar_field);
LINK_ENTITY_TO_GAME(func_pendulum);
LINK_ENTITY_TO_GAME(func_plat);
LINK_ENTITY_TO_GAME(func_platrot);
LINK_ENTITY_TO_GAME(func_pushable);
LINK_ENTITY_TO_GAME(func_recharge);
LINK_ENTITY_TO_GAME(func_rot_button);
LINK_ENTITY_TO_GAME(func_rotating);
LINK_ENTITY_TO_GAME(func_tank);
LINK_ENTITY_TO_GAME(func_tankcontrols);
LINK_ENTITY_TO_GAME(func_tanklaser);
LINK_ENTITY_TO_GAME(func_tankmortar);
LINK_ENTITY_TO_GAME(func_tankrocket);
LINK_ENTITY_TO_GAME(func_trackautochange);
LINK_ENTITY_TO_GAME(func_trackchange);
LINK_ENTITY_TO_GAME(func_tracktrain);
LINK_ENTITY_TO_GAME(func_train);
LINK_ENTITY_TO_GAME(func_traincontrols);
LINK_ENTITY_TO_GAME(func_wall);
LINK_ENTITY_TO_GAME(func_wall_toggle);
LINK_ENTITY_TO_GAME(func_water);
LINK_ENTITY_TO_GAME(game_counter);
LINK_ENTITY_TO_GAME(game_counter_set);
LINK_ENTITY_TO_GAME(game_end);
LINK_ENTITY_TO_GAME(game_player_equip);
LINK_ENTITY_TO_GAME(game_player_hurt);
LINK_ENTITY_TO_GAME(game_player_team);
LINK_ENTITY_TO_GAME(game_score);
LINK_ENTITY_TO_GAME(game_team_master);
LINK_ENTITY_TO_GAME(game_team_set);
LINK_ENTITY_TO_GAME(game_text);
LINK_ENTITY_TO_GAME(game_zone_player);
LINK_ENTITY_TO_GAME(garg_stomp);
LINK_ENTITY_TO_GAME(gibshooter);
LINK_ENTITY_TO_GAME(grenade);
LINK_ENTITY_TO_GAME(hornet);
LINK_ENTITY_TO_GAME(hvr_rocket);
LINK_ENTITY_TO_GAME(info_bigmomma);
LINK_ENTITY_TO_GAME(info_intermission);
LINK_ENTITY_TO_GAME(info_landmark);
LINK_ENTITY_TO_GAME(info_node);
LINK_ENTITY_TO_GAME(info_node_air);
LINK_ENTITY_TO_GAME(info_null);
LINK_ENTITY_TO_GAME(info_player_deathmatch);
LINK_ENTITY_TO_GAME(info_player_start);
LINK_ENTITY_TO_GAME(info_target);
LINK_ENTITY_TO_GAME(info_teleport_destination);
LINK_ENTITY_TO_GAME(infodecal);
LINK_ENTITY_TO_GAME(item_airtank);
LINK_ENTITY_TO_GAME(item_antidote);
LINK_ENTITY_TO_GAME(item_battery);
LINK_ENTITY_TO_GAME(item_healthkit);
LINK_ENTITY_TO_GAME(item_longjump);
LINK_ENTITY_TO_GAME(item_security);
LINK_ENTITY_TO_GAME(item_sodacan);
LINK_ENTITY_TO_GAME(item_suit);
LINK_ENTITY_TO_GAME(laser_spot);
LINK_ENTITY_TO_GAME(light);
LINK_ENTITY_TO_GAME(light_environment);
LINK_ENTITY_TO_GAME(light_spot);
LINK_ENTITY_TO_GAME(momentary_door);
LINK_ENTITY_TO_GAME(momentary_rot_button);
LINK_ENTITY_TO_GAME(monster_alien_controller);
LINK_ENTITY_TO_GAME(monster_alien_grunt);
LINK_ENTITY_TO_GAME(monster_alien_slave);
LINK_ENTITY_TO_GAME(monster_apache);
LINK_ENTITY_TO_GAME(monster_babycrab);
LINK_ENTITY_TO_GAME(monster_barnacle);
LINK_ENTITY_TO_GAME(monster_barney);
LINK_ENTITY_TO_GAME(monster_barney_dead);
LINK_ENTITY_TO_GAME(monster_bigmomma);
LINK_ENTITY_TO_GAME(monster_bloater);
LINK_ENTITY_TO_GAME(monster_bullchicken);
LINK_ENTITY_TO_GAME(monster_cine2_hvyweapons);
LINK_ENTITY_TO_GAME(monster_cine2_scientist);
LINK_ENTITY_TO_GAME(monster_cine2_slave);
LINK_ENTITY_TO_GAME(monster_cine3_barney);
LINK_ENTITY_TO_GAME(monster_cine3_scientist);
LINK_ENTITY_TO_GAME(monster_cine_barney);
LINK_ENTITY_TO_GAME(monster_cine_panther);
LINK_ENTITY_TO_GAME(monster_cine_scientist);
LINK_ENTITY_TO_GAME(monster_cockroach);
LINK_ENTITY_TO_GAME(monster_flyer);
LINK_ENTITY_TO_GAME(monster_flyer_flock);
LINK_ENTITY_TO_GAME(monster_furniture);
LINK_ENTITY_TO_GAME(monster_gargantua);
LINK_ENTITY_TO_GAME(monster_generic);
LINK_ENTITY_TO_GAME(monster_gman);
LINK_ENTITY_TO_GAME(monster_grunt_repel);
LINK_ENTITY_TO_GAME(monster_headcrab);
LINK_ENTITY_TO_GAME(monster_hevsuit_dead);
LINK_ENTITY_TO_GAME(monster_hgrunt_dead);
LINK_ENTITY_TO_GAME(monster_houndeye);
LINK_ENTITY_TO_GAME(monster_human_assassin);
LINK_ENTITY_TO_GAME(monster_human_grunt);
LINK_ENTITY_TO_GAME(monster_ichthyosaur);
LINK_ENTITY_TO_GAME(monster_leech);
LINK_ENTITY_TO_GAME(monster_miniturret);
LINK_ENTITY_TO_GAME(monster_mortar);
LINK_ENTITY_TO_GAME(monster_nihilanth);
LINK_ENTITY_TO_GAME(monster_osprey);
LINK_ENTITY_TO_GAME(monster_rat);
LINK_ENTITY_TO_GAME(monster_satchel);
LINK_ENTITY_TO_GAME(monster_scientist);
LINK_ENTITY_TO_GAME(monster_scientist_dead);
LINK_ENTITY_TO_GAME(monster_sentry);
LINK_ENTITY_TO_GAME(monster_sitting_scientist);
LINK_ENTITY_TO_GAME(monster_snark);
LINK_ENTITY_TO_GAME(monster_tentacle);
LINK_ENTITY_TO_GAME(monster_tentaclemaw);
LINK_ENTITY_TO_GAME(monster_tripmine);
LINK_ENTITY_TO_GAME(monster_turret);
LINK_ENTITY_TO_GAME(monster_vortigaunt);
LINK_ENTITY_TO_GAME(monster_zombie);
LINK_ENTITY_TO_GAME(monstermaker);
LINK_ENTITY_TO_GAME(multi_manager);
LINK_ENTITY_TO_GAME(multisource);
LINK_ENTITY_TO_GAME(nihilanth_energy_ball);
LINK_ENTITY_TO_GAME(node_viewer);
LINK_ENTITY_TO_GAME(node_viewer_fly);
LINK_ENTITY_TO_GAME(node_viewer_human);
LINK_ENTITY_TO_GAME(node_viewer_large);
LINK_ENTITY_TO_GAME(path_corner);
LINK_ENTITY_TO_GAME(path_track);
LINK_ENTITY_TO_GAME(player);
LINK_ENTITY_TO_GAME(player_loadsaved);
LINK_ENTITY_TO_GAME(player_weaponstrip);
LINK_ENTITY_TO_GAME(rpg_rocket);
LINK_ENTITY_TO_GAME(scripted_sentence);
LINK_ENTITY_TO_GAME(scripted_sequence);
LINK_ENTITY_TO_GAME(soundent);
LINK_ENTITY_TO_GAME(spark_shower);
LINK_ENTITY_TO_GAME(speaker);
LINK_ENTITY_TO_GAME(squidspit);
LINK_ENTITY_TO_GAME(streak_spiral);
LINK_ENTITY_TO_GAME(target_cdaudio);
LINK_ENTITY_TO_GAME(test_effect);
LINK_ENTITY_TO_GAME(testhull);
LINK_ENTITY_TO_GAME(trigger);
LINK_ENTITY_TO_GAME(trigger_auto);
LINK_ENTITY_TO_GAME(trigger_autosave);
LINK_ENTITY_TO_GAME(trigger_camera);
LINK_ENTITY_TO_GAME(trigger_cdaudio);
LINK_ENTITY_TO_GAME(trigger_changelevel);
LINK_ENTITY_TO_GAME(trigger_changetarget);
LINK_ENTITY_TO_GAME(trigger_counter);
LINK_ENTITY_TO_GAME(trigger_endsection);
LINK_ENTITY_TO_GAME(trigger_gravity);
LINK_ENTITY_TO_GAME(trigger_hurt);
LINK_ENTITY_TO_GAME(trigger_monsterjump);
LINK_ENTITY_TO_GAME(trigger_multiple);
LINK_ENTITY_TO_GAME(trigger_once);
LINK_ENTITY_TO_GAME(trigger_push);
LINK_ENTITY_TO_GAME(trigger_relay);
LINK_ENTITY_TO_GAME(trigger_teleport);
LINK_ENTITY_TO_GAME(trigger_transition);
LINK_ENTITY_TO_GAME(weapon_357);
LINK_ENTITY_TO_GAME(weapon_9mmAR);
LINK_ENTITY_TO_GAME(weapon_9mmhandgun);
LINK_ENTITY_TO_GAME(weapon_crossbow);
LINK_ENTITY_TO_GAME(weapon_crowbar);
LINK_ENTITY_TO_GAME(weapon_egon);
LINK_ENTITY_TO_GAME(weapon_gauss);
LINK_ENTITY_TO_GAME(weapon_glock);
LINK_ENTITY_TO_GAME(weapon_handgrenade);
LINK_ENTITY_TO_GAME(weapon_hornetgun);
LINK_ENTITY_TO_GAME(weapon_mp5);
LINK_ENTITY_TO_GAME(weapon_python);
LINK_ENTITY_TO_GAME(weapon_rpg);
LINK_ENTITY_TO_GAME(weapon_satchel);
LINK_ENTITY_TO_GAME(weapon_shotgun);
LINK_ENTITY_TO_GAME(weapon_snark);
LINK_ENTITY_TO_GAME(weapon_tripmine);
LINK_ENTITY_TO_GAME(weaponbox);
LINK_ENTITY_TO_GAME(world_items);
LINK_ENTITY_TO_GAME(worldspawn);
LINK_ENTITY_TO_GAME(xen_hair);
LINK_ENTITY_TO_GAME(xen_hull);
LINK_ENTITY_TO_GAME(xen_plantlight);
LINK_ENTITY_TO_GAME(xen_spore_large);
LINK_ENTITY_TO_GAME(xen_spore_medium);
LINK_ENTITY_TO_GAME(xen_spore_small);
LINK_ENTITY_TO_GAME(xen_tree);
LINK_ENTITY_TO_GAME(xen_ttrigger);

// additional entities for Counter-Strike
LINK_ENTITY_TO_GAME(ammo_338magnum);
LINK_ENTITY_TO_GAME(ammo_357sig);
LINK_ENTITY_TO_GAME(ammo_45acp);
LINK_ENTITY_TO_GAME(ammo_50ae);
LINK_ENTITY_TO_GAME(ammo_556nato);
LINK_ENTITY_TO_GAME(ammo_556natobox);
LINK_ENTITY_TO_GAME(ammo_57mm);
LINK_ENTITY_TO_GAME(ammo_762nato);
LINK_ENTITY_TO_GAME(ammo_9mm);
LINK_ENTITY_TO_GAME(armoury_entity);
LINK_ENTITY_TO_GAME(env_bombglow);
LINK_ENTITY_TO_GAME(env_rain);
LINK_ENTITY_TO_GAME(env_snow);
LINK_ENTITY_TO_GAME(func_bomb_target);
LINK_ENTITY_TO_GAME(func_buyzone);
LINK_ENTITY_TO_GAME(func_escapezone);
LINK_ENTITY_TO_GAME(func_grencatch);
LINK_ENTITY_TO_GAME(func_hostage_rescue);
LINK_ENTITY_TO_GAME(func_rain);
LINK_ENTITY_TO_GAME(func_snow);
LINK_ENTITY_TO_GAME(func_vehicle);
LINK_ENTITY_TO_GAME(func_vehiclecontrols);
LINK_ENTITY_TO_GAME(func_vip_safetyzone);
LINK_ENTITY_TO_GAME(func_weaponcheck);
LINK_ENTITY_TO_GAME(hostage_entity);
LINK_ENTITY_TO_GAME(info_bomb_target);
LINK_ENTITY_TO_GAME(info_hostage_rescue);
LINK_ENTITY_TO_GAME(info_map_parameters);
LINK_ENTITY_TO_GAME(info_vip_start);
LINK_ENTITY_TO_GAME(item_assaultsuit);
LINK_ENTITY_TO_GAME(item_kevlar);
LINK_ENTITY_TO_GAME(item_thighpack);
LINK_ENTITY_TO_GAME(weapon_ak47);
LINK_ENTITY_TO_GAME(weapon_aug);
LINK_ENTITY_TO_GAME(weapon_awp);
LINK_ENTITY_TO_GAME(weapon_c4);
LINK_ENTITY_TO_GAME(weapon_deagle);
LINK_ENTITY_TO_GAME(weapon_elite);
LINK_ENTITY_TO_GAME(weapon_famas);
LINK_ENTITY_TO_GAME(weapon_fiveseven);
LINK_ENTITY_TO_GAME(weapon_flashbang);
LINK_ENTITY_TO_GAME(weapon_galil);
LINK_ENTITY_TO_GAME(weapon_g3sg1);
LINK_ENTITY_TO_GAME(weapon_glock18);
LINK_ENTITY_TO_GAME(weapon_hegrenade);
LINK_ENTITY_TO_GAME(weapon_knife);
LINK_ENTITY_TO_GAME(weapon_m249);
LINK_ENTITY_TO_GAME(weapon_m3);
LINK_ENTITY_TO_GAME(weapon_m4a1);
LINK_ENTITY_TO_GAME(weapon_mac10);
LINK_ENTITY_TO_GAME(weapon_mp5navy);
LINK_ENTITY_TO_GAME(weapon_p228);
LINK_ENTITY_TO_GAME(weapon_p90);
LINK_ENTITY_TO_GAME(weapon_scout);
LINK_ENTITY_TO_GAME(weapon_sg550);
LINK_ENTITY_TO_GAME(weapon_sg552);
LINK_ENTITY_TO_GAME(weapon_shield);
LINK_ENTITY_TO_GAME(weapon_shieldgun);
LINK_ENTITY_TO_GAME(weapon_smokegrenade);
LINK_ENTITY_TO_GAME(weapon_tmp);
LINK_ENTITY_TO_GAME(weapon_ump45);
LINK_ENTITY_TO_GAME(weapon_usp);
LINK_ENTITY_TO_GAME(weapon_xm1014);

// additional entities for Opposing Force
LINK_ENTITY_TO_GAME(ammo_556);
LINK_ENTITY_TO_GAME(ammo_762);
LINK_ENTITY_TO_GAME(ammo_eagleclip);
LINK_ENTITY_TO_GAME(ammo_spore);
LINK_ENTITY_TO_GAME(charged_bolt);
LINK_ENTITY_TO_GAME(ctf_hudicon);
LINK_ENTITY_TO_GAME(displacer_ball);
LINK_ENTITY_TO_GAME(eagle_laser);
LINK_ENTITY_TO_GAME(env_blowercannon);
LINK_ENTITY_TO_GAME(env_electrified_wire);
LINK_ENTITY_TO_GAME(env_genewormcloud);
LINK_ENTITY_TO_GAME(env_genewormspawn);
LINK_ENTITY_TO_GAME(env_rope);
LINK_ENTITY_TO_GAME(env_spritetrain);
LINK_ENTITY_TO_GAME(func_op4mortarcontroller);
LINK_ENTITY_TO_GAME(func_tank_of);
LINK_ENTITY_TO_GAME(func_tankcontrols_of);
LINK_ENTITY_TO_GAME(func_tanklaser_of);
LINK_ENTITY_TO_GAME(func_tankmortar_of);
LINK_ENTITY_TO_GAME(func_tankrocket_of);
LINK_ENTITY_TO_GAME(gonomeguts);
LINK_ENTITY_TO_GAME(grapple_tip);
LINK_ENTITY_TO_GAME(hvr_blkop_rocket);
LINK_ENTITY_TO_GAME(info_ctfdetect);
LINK_ENTITY_TO_GAME(info_ctfspawn);
LINK_ENTITY_TO_GAME(info_ctfspawn_powerup);
LINK_ENTITY_TO_GAME(info_displacer_earth_target);
LINK_ENTITY_TO_GAME(info_displacer_xen_target);
LINK_ENTITY_TO_GAME(info_pitworm);
LINK_ENTITY_TO_GAME(info_pitworm_steam_lock);
LINK_ENTITY_TO_GAME(item_ctfaccelerator);
LINK_ENTITY_TO_GAME(item_ctfbackpack);
LINK_ENTITY_TO_GAME(item_ctfbase);
LINK_ENTITY_TO_GAME(item_ctfflag);
LINK_ENTITY_TO_GAME(item_ctflongjump);
LINK_ENTITY_TO_GAME(item_ctfportablehev);
LINK_ENTITY_TO_GAME(item_ctfregeneration);
LINK_ENTITY_TO_GAME(item_generic);
LINK_ENTITY_TO_GAME(item_nuclearbomb);
LINK_ENTITY_TO_GAME(item_nuclearbombbutton);
LINK_ENTITY_TO_GAME(item_nuclearbombtimer);
LINK_ENTITY_TO_GAME(item_vest);
LINK_ENTITY_TO_GAME(monster_ShockTrooper_dead);
LINK_ENTITY_TO_GAME(monster_alien_babyvoltigore);
LINK_ENTITY_TO_GAME(monster_alien_slave_dead);
LINK_ENTITY_TO_GAME(monster_alien_voltigore);
LINK_ENTITY_TO_GAME(monster_assassin_repel);
LINK_ENTITY_TO_GAME(monster_blkop_apache);
LINK_ENTITY_TO_GAME(monster_blkop_osprey);
LINK_ENTITY_TO_GAME(monster_cleansuit_scientist);
LINK_ENTITY_TO_GAME(monster_cleansuit_scientist_dead);
LINK_ENTITY_TO_GAME(monster_drillsergeant);
LINK_ENTITY_TO_GAME(monster_fgrunt_repel);
LINK_ENTITY_TO_GAME(monster_geneworm);
LINK_ENTITY_TO_GAME(monster_gonome);
LINK_ENTITY_TO_GAME(monster_gonome_dead);
LINK_ENTITY_TO_GAME(monster_grunt_ally_repel);
LINK_ENTITY_TO_GAME(monster_hfgrunt_dead);
LINK_ENTITY_TO_GAME(monster_houndeye_dead);
LINK_ENTITY_TO_GAME(monster_human_friendly_grunt);
LINK_ENTITY_TO_GAME(monster_human_grunt_ally);
LINK_ENTITY_TO_GAME(monster_human_grunt_ally_dead);
LINK_ENTITY_TO_GAME(monster_human_medic_ally);
LINK_ENTITY_TO_GAME(monster_human_torch_ally);
LINK_ENTITY_TO_GAME(monster_male_assassin);
LINK_ENTITY_TO_GAME(monster_massassin_dead);
LINK_ENTITY_TO_GAME(monster_medic_ally_repel);
LINK_ENTITY_TO_GAME(monster_op4loader);
LINK_ENTITY_TO_GAME(monster_otis);
LINK_ENTITY_TO_GAME(monster_otis_dead);
LINK_ENTITY_TO_GAME(monster_penguin);
LINK_ENTITY_TO_GAME(monster_pitdrone);
LINK_ENTITY_TO_GAME(monster_pitworm);
LINK_ENTITY_TO_GAME(monster_pitworm_up);
LINK_ENTITY_TO_GAME(monster_recruit);
LINK_ENTITY_TO_GAME(monster_shockroach);
LINK_ENTITY_TO_GAME(monster_shocktrooper);
LINK_ENTITY_TO_GAME(monster_shocktrooper_repel);
LINK_ENTITY_TO_GAME(monster_sitting_cleansuit_scientist);
LINK_ENTITY_TO_GAME(monster_skeleton_dead);
LINK_ENTITY_TO_GAME(monster_torch_ally_repel);
LINK_ENTITY_TO_GAME(monster_zombie_barney);
LINK_ENTITY_TO_GAME(monster_zombie_soldier);
LINK_ENTITY_TO_GAME(monster_zombie_soldier_dead);
LINK_ENTITY_TO_GAME(mortar_shell);
LINK_ENTITY_TO_GAME(op4mortar);
LINK_ENTITY_TO_GAME(pitdronespike);
LINK_ENTITY_TO_GAME(pitworm_gib);
LINK_ENTITY_TO_GAME(pitworm_gibshooter);
LINK_ENTITY_TO_GAME(rope_sample);
LINK_ENTITY_TO_GAME(rope_segment);
LINK_ENTITY_TO_GAME(shock_beam);
LINK_ENTITY_TO_GAME(spore);
LINK_ENTITY_TO_GAME(trigger_ctfgeneric);
LINK_ENTITY_TO_GAME(trigger_geneworm_hit);
LINK_ENTITY_TO_GAME(trigger_kill_nogib);
LINK_ENTITY_TO_GAME(trigger_playerfreeze);
LINK_ENTITY_TO_GAME(trigger_xen_return);
LINK_ENTITY_TO_GAME(weapon_displacer);
LINK_ENTITY_TO_GAME(weapon_eagle);
LINK_ENTITY_TO_GAME(weapon_grapple);
LINK_ENTITY_TO_GAME(weapon_penguin);
LINK_ENTITY_TO_GAME(weapon_pipewrench);
LINK_ENTITY_TO_GAME(weapon_shockrifle);
LINK_ENTITY_TO_GAME(weapon_shockroach);
LINK_ENTITY_TO_GAME(weapon_sniperrifle);
LINK_ENTITY_TO_GAME(weapon_sporelauncher);

// additional entities for Redemption
LINK_ENTITY_TO_GAME(monster_roy);

// additional entities for Ricochet
LINK_ENTITY_TO_GAME(disc);
LINK_ENTITY_TO_GAME(disc_arena);
LINK_ENTITY_TO_GAME(discarena);
LINK_ENTITY_TO_GAME(func_disctoggle);
LINK_ENTITY_TO_GAME(func_plat_toggleremove);
LINK_ENTITY_TO_GAME(info_player_spectator);
LINK_ENTITY_TO_GAME(item_powerup);
LINK_ENTITY_TO_GAME(player_weaponship);
LINK_ENTITY_TO_GAME(trigger_discerturn);
LINK_ENTITY_TO_GAME(trigger_discreturn);
LINK_ENTITY_TO_GAME(trigger_fall);
LINK_ENTITY_TO_GAME(trigger_jump);
LINK_ENTITY_TO_GAME(trip_beam);
LINK_ENTITY_TO_GAME(weapon_disc);

