using UnityEngine;
using System.Collections;

public class Constants : MonoBehaviour {
	
	public enum 				WEAPONS:int 			{ PISTOL = 0,	SHOTGUN = 1,	COUNT=2};
	public static float [] 		WeaponDelayTimeCoef = 	{ 1f,			0.75f};

	public static WEAPONS		WEAPON = WEAPONS.PISTOL;

	public enum 				COLUMN_POS_X	{ LEFT = 0, 	CENTER = 1, 	RIGHT = 2 };
	public static float []		ColumnPos = 	{ -1.25f, 		0f,				1.25f};
		
}
