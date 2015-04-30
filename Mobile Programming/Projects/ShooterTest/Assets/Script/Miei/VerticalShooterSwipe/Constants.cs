using UnityEngine;
using System.Collections;

public class Constants : MonoBehaviour {
	
	public enum 				Weapons:int 			{ PISTOL = 0,	SHOTGUN = 1,	COUNT=2};
	public static float [] 		WeaponDelayTimeCoef = 	{ 1f,			0.75f};

	public static Weapons		WEAPON = Weapons.PISTOL;
		
}
