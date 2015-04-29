using UnityEngine;
using UnityEngine.UI;
using System.Collections;


public class NativeBridge : MonoBehaviour {
	
	#if UNITY_ANDROID
	AndroidJavaClass androidClass;
	#endif

	public Text textTest;
	public string fixedText = "Welcome in Android\n";
	public string fixedTouchCount = "Touch Count: ";

	void Start (){
		textTest.text = fixedText;
		#if UNITY_ANDROID
		AndroidJNI.AttachCurrentThread();
		androidClass = new AndroidJavaClass("com.master.plugin.UnityBridge");
		#endif
	}
	
	#if UNITY_ANDROID
	public void checkMyStringOnJava(){
		string myString = androidClass.CallStatic<string>("getMyString");
		int myCount = androidClass.CallStatic<int>("getTouchCount");
		textTest.text = fixedText+myString+"\n"+fixedTouchCount+myCount;
	}
	#endif
	
	void Update()
	{		
		if (Input.GetMouseButtonDown (0)) {

			#if UNITY_ANDROID
			checkMyStringOnJava();
//			object[] args = new[] { Random.Range(1,100), this.gameObject.name, "checkMyIntOnJava" };
//			androidClass.CallStatic("callbackToUnityMethod", args);
			#endif
		}
	}
}
