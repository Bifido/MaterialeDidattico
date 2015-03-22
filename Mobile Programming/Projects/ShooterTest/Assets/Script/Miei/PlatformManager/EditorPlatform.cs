#if UNITY_EDITOR
using UnityEngine;
using System.Collections;

public class EditorPlatform: ValePlatformManager{
	public EditorPlatform(){
		Debug.Log("EDITOR!");
	}
}
#endif