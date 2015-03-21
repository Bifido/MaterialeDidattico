#if UNITY_ANDROID
using UnityEngine;
using System.Collections;

public class AndroidPlatform : PlatformManager {

	public AndroidPlatform(){
		System.out.println("ANDROID!!!");
	}
}
#endif