using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;
using System;



public class JavaPluginLoader : MonoBehaviour {
	void Start(){
		AndroidJavaClass jc = new AndroidJavaClass("ReturnClassPlugin.MainActivity");
		jc.CallStatic("ReturnString");
	}
}
