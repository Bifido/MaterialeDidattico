using UnityEngine;
using System.Collections;

/*
Università di Verona - Master Videogame programming 2015
Corso Mobile programming - Roberto Mangiafico

Excercise: Having trouble figuring out what's causing an intermittent bug. The issue is that sometimes the "explosionA" animation 
will trigger multiple times when either holding the A key or pressing it rapidly, but not always.

Base Solution
 */

public class Shooter : MonoBehaviour 
{
	void Start(){
		fireTimer = 0;
		this.pm = PlatformManager.getPlatformManager();
	}
	
	void Update(){
		if(Time.time > fireTimer){
			Cast();
		}
	}
	
	void Cast() 
	{
		if (Input.GetKey(KeyCode.A))
		{
			Debug.Log("explosion");
			fireTimer = Time.time + rechargeTime;
		}
	}

	//VARS
	[SerializeField] private bool canFire = true;
	[SerializeField] private float 	rechargeTime = 0.5f;
	[SerializeField] private float fireTimer;
	[SerializeField] private PlatformManager pm;
}
