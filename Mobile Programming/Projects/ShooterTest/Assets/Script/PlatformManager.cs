using UnityEngine;
using System.Collections;

public class PlatformManager{
	private static PlatformManager instance;

	protected PlatformManager(){}
	public static PlatformManager getPlatformManager(){
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
