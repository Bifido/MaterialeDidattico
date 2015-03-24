using UnityEngine;
using System.Collections;

public class MyProjectile : MonoBehaviour,Executable{

	//---------------EXECUTABLE INTERFACE
	public void Execute(){
		this.m_tStartingExecutionTime = 0;
		this.enabled = true;
	}

	public void StopExecute(){
		this.enabled = false;
	}

	public bool isExecuting(){
		return this.enabled;
	}
	//---------------EXECUTABLE INTERFACE

	// Use this for initialization
	void Start () {
		m_vDirection = this.transform.forward;
		this.StopExecute();
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
			this.StopExecute();
		}
	}

	protected virtual void ComputeTrajectory(){
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
