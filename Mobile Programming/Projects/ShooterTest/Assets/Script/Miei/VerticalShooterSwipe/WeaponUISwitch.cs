using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class WeaponUISwitch : MonoBehaviour {
	
	public Text WeaponSwitcherText;

	// Use this for initialization
	void Start () {
		this.WeaponSwitcherText.text = Constants.WEAPON.ToString();
	}
	
	public void NextWeapon(){
		if(Constants.WEAPON == Constants.WEAPONS.COUNT-1){
			Constants.WEAPON = 0;
		}else{
			Constants.WEAPON++;
		}
		this.WeaponSwitcherText.text = Constants.WEAPON.ToString().ToUpper();
	}
}
