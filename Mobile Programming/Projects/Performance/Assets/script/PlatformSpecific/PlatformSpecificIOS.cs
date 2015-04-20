using UnityEngine;
using System.Collections;

public class PlatformSpecificIOS : PlatformSpecificBase
{
	#if UNITY_IOS
	public override void InitPlatofrm()
	{
		base.InitPlatofrm();

		NativeServiceIOS.Init();

		Debug.Log("IOS Init");
	}
	
	public override float GetShootingInterval()
	{
		return NativeServiceIOS.GetFloat();
	}

	public override string GetPlatformStatus()
	{
		return NativeServiceIOS.GetStatus();
	}

	#endif
}
