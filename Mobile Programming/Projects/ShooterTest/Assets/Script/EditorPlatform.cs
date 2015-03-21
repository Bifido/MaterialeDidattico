#if UNITY_EDITOR
using UnityEngine;
using System.Collections;

public class EditorPlatform: PlatformManager{
	public EditorPlatform(){
		Debug.Log("EDITOR!");
	}
}
#endif