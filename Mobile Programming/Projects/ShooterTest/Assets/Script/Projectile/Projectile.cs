using UnityEngine;
using System.Collections;

public class Projectile : MonoBehaviour {

	// Use this for initialization
	void Start () {
		m_vDirection = this.transform.forward;
	}
	
	// Update is called once per frame
	void Update () {
		if(m_bShooted){
			this.ComputeTrajectory();
		}
	}

	private void ComputeTrajectory(){
		this.transform.position += m_vDirection * (m_fSpeed * Time.deltaTime);
	}

	[SerializeField] private float 		m_fSpeed = 1.0f;

	private bool 						m_bShooted = false;
	private Vector3						m_vDirection = Vector3.one;
}
