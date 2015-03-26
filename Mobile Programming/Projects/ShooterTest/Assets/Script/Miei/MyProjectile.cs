using UnityEngine;
using System.Collections;

public class MyProjectile : MonoBehaviour,Executable{

	//---------------EXECUTABLE INTERFACE
	public virtual void Execute(){
		this.transform.localPosition = this.gameObject.transform.parent.transform.localPosition;
		this.m_tStartingExecutionTime = 0;
		this.enabled = true;
	}

	public void StopExecute(){
		this.enabled = false;
	}

	public bool IsExecuting(){
		return this.enabled;
	}

	public void SetExcutioner(Executioner exec){
		this.m_iExecutioner = exec;
	}

	public GameObject GetGameObject(){
		return this.gameObject;
	}

	public void NotifyExecutioner(Executable executable,bool success){
		if(success){
			this.m_iExecutioner.NotifyExecuteEndSuccess(this);
		}else{
			this.m_iExecutioner.NotifyExecuteEndFail(this);
		}
	}

	public void SetPool(Pool<Executable> pool){
		this.m_oCart = pool;
	}
	public Pool<Executable> GetPool(){
		return this.m_oCart;
	}

	//---------------EXECUTABLE INTERFACE

	// Use this for initialization
	void Start () {
		m_vDirection = this.transform.forward;
		this.StopExecute();
	}
	
	// Update is called once per frame
	void Update () {
		this.m_tStartingExecutionTime += Time.deltaTime;
		if(m_tStartingExecutionTime < this.m_fTimeToLife){
			ComputeTrajectory();
		}else{
			this.StopExecute();
			this.NotifyExecutioner(this,false);
		}
	}

	void OnTriggerEnter(Collider other){
		if(other.tag.Equals("Target")){
			this.NotifyExecutioner(this,true);
		}
	}
	
	protected virtual void ComputeTrajectory(){
		Debug.Log ( "compute base trajectory");
		this.transform.position += m_vDirection * (m_fSpeed * Time.deltaTime);
	}

	[SerializeField] protected float 		m_fSpeed;
	[SerializeField] private Vector3	m_vDirection;

	//this is the starting value of projectile's life
	[SerializeField] private float		m_fTimeToLife;
	private float 						m_tStartingExecutionTime = 0f;
	private Executioner					m_iExecutioner;
	private Pool<Executable> 			m_oCart;

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
