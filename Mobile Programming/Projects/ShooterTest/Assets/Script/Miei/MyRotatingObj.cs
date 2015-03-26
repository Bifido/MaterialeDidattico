using UnityEngine;
using System.Collections;

public class MyRotatingObj : MyProjectile {

	private Vector3 m_vRotation = new Vector3(0.5f,0.5f,0.5f);

	protected virtual void ComputeTrajectory(){
		Debug.Log("rotating!");
		base.ComputeTrajectory();
		this.transform.Rotate(m_vRotation * m_fSpeed * Time.deltaTime);
	}
}
