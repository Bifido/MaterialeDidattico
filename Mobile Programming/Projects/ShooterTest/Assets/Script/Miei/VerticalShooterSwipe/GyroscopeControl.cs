using UnityEngine;
using System.Collections;

public class GyroscopeControl : MonoBehaviour {

	private Vector3 forceVec = new Vector3(1f,0f,0f);
	public Rigidbody rb;
	void Start() {
//		rb = GetComponent<Rigidbody>();
	}
	void FixedUpdate() {
		Debug.Log(Input.gyro.userAcceleration);
		rb.AddForce(Input.gyro.userAcceleration.x * forceVec);
	}
}
