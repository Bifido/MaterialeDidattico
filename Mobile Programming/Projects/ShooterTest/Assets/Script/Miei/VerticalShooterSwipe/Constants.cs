using UnityEngine;
using System.Collections;

public class Constants : MonoBehaviour {

	public static float []		COLUMN_POS_A = 	{ -1.25f, 		0f,				1.25f};
	public enum 				COLUMN_POS_X	{ LEFT = 0, 	CENTER = 1, 	RIGHT = 2	,COUNT = 3};
	
	public static float			OBSTACLE_SPEED = 1.5f;

	public static int			ENEMY_POOL_ELEMENTS = 5;
	public static float			ENEMY_SPAWN_TIMER = 4f;

	public static WEAPONS		WEAPON = 					WEAPONS.PISTOL;
	public enum 				WEAPONS:int 				{ PISTOL = 0,	SHOTGUN = 1,	COUNT=2};
	public static float [] 		WEAPON_DELAY_TIME_COEF = 	{ 1f,			0.75f};
}
