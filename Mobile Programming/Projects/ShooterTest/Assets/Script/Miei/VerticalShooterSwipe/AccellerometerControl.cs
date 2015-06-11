using UnityEngine;
using System.Collections;

public class AccellerometerControl : MonoBehaviour {

	private Vector3 forceVec = new Vector3(4f,0f,0f);
	public Rigidbody rb;
	void Start() {
//		rb = GetComponent<Rigidbody>();
	}
	void FixedUpdate() {
		rb.AddForce(Input.acceleration.x * forceVec);
	}
}
