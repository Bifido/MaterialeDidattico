using UnityEngine;
using System.Collections;

public class Constants : MonoBehaviour {

	public static float			BACKGROUND_SPEED = 0.75f;
	public static float			BACKGROUND_WALL_X_POS = -3.1f;
	public static float			BACKGROUND_WALL_Y_OFFSET = 8.5f; 
	public static int	 		BACKGROUND_WALL_NUMBER = 3;
	public static Vector3		BACKGROUND_WALL_SPAWN_POS = new Vector3(BACKGROUND_WALL_X_POS,
	                                                               		BACKGROUND_WALL_Y_OFFSET*(BACKGROUND_WALL_NUMBER-1),
	                                                               		0f);
	public static float			BACKGROUND_WALL_Y_LIMIT = -9f;

	public static float []		COLUMN_POS_A = 	{ -1.25f, 		0f,				1.25f};
	public enum 				COLUMN_POS_X	{ LEFT = 0, 	CENTER = 1, 	RIGHT = 2	,COUNT = 3};
	
	public static float			OBSTACLE_SPEED = BACKGROUND_SPEED*2;

	public static int			ENEMY_POOL_ELEMENTS = 5;
	public static float			ENEMY_SPAWN_TIMER = 4f;

	public static WEAPONS		WEAPON = 					WEAPONS.PISTOL;
	public enum 				WEAPONS:int 				{ PISTOL = 0,	SHOTGUN = 1,	COUNT=2};
	public static float [] 		WEAPON_DELAY_TIME_COEF = 	{ 1f,			0.75f};
}
