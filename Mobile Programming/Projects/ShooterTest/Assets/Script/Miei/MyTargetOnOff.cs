using UnityEngine;
using System.Collections;

public class MyTargetOnOff : MonoBehaviour {

	public GameObject TargetGo;

	// Use this for initialization
	void Start () {
		this.TargetGo.SetActive(false);
	}
	
	// Update is called once per frame
	void Update () {
		if(Input.GetKeyDown(KeyCode.C)){
			this.TargetGo.SetActive(!this.TargetGo.activeSelf);
		}
	}
}
