using UnityEngine;
using System.Collections;

public class ValePlatformManager{
	private static ValePlatformManager instance;

	protected ValePlatformManager(){}
	public static ValePlatformManager getPlatformManager(){
		if(instance == null){
			#if UNITY_ANDROID
			instance = new AndroidPlatform();
			#elif UNITY_EDITOR
			instance = new EditorPlatform();
			#endif
		}
		return instance;
	}
}
