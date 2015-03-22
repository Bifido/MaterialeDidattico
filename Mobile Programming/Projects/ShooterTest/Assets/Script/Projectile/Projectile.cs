using UnityEngine;
using System.Collections;

public class Projectile : Executable {

	public override void Execute(){
		this.m_tStartingExecutionTime = 0;
		base.Execute();
	}

	// Use this for initialization
	void Start () {
		m_vDirection = this.transform.forward;

		//TODO remove after test
		this.Execute();
//		base.StopExecute();
	}
	
	// Update is called once per frame
	void Update () {
//		Check not more required implementing executable
//		if(m_bShooted){
//			this.ComputeTrajectory();
//		}
		this.m_tStartingExecutionTime += Time.deltaTime;
		if(m_tStartingExecutionTime < this.m_fTimeToLife){
			//TODO check collisione
			this.ComputeTrajectory();
		}else{
			base.StopExecute();
		}
	}

	private void ComputeTrajectory(){
		this.transform.position += m_vDirection * (m_fSpeed * Time.deltaTime);
	}

	[SerializeField] private float 		m_fSpeed;
	[SerializeField] private Vector3	m_vDirection;

	//this is the starting value of projectile's life
	[SerializeField] private float		m_fTimeToLife;
	private float 						m_tStartingExecutionTime = 0f;

//	don't required anymore
//	private bool 						m_bShooted = false;

	//before execute need to set direction
	public Vector3 Direction{
		get {
			return this.m_vDirection;
		}
		set{
			this.m_vDirection = value;
		}
	}
}
