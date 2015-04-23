using UnityEngine;
using System.Collections;

public class CameraCenter : MonoBehaviour {

	public Camera cam;

	// Use this for initialization
	void Start () {
		cam.transform.position = new Vector3(Screen.width/2,Screen.height/2,-10f);
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
