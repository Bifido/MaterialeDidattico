using UnityEngine;
using System.Collections;

public class EnemyMovement : MonoBehaviour,Executable,Allocable {

	//EXECUTABLE INTERFACE-------------------

	public void Execute(){
		this.m_fLife = 0f;
		this.m_bCollided = false;
		this.enabled = true;
	}
	public void StopExecute(){
		this.enabled = false;
	}
	public bool IsExecuting(){
		return this.enabled;
	}	
	public void SetExcutioner(Executioner exec,int ID){
		this.m_oExecutioner = exec;
	}
	public void NotifyExecutioner(bool success){
		if(success){
			this.m_oExecutioner.NotifyExecuteEndSuccess(this);
		}else{
			this.GetComponent<Collider>().isTrigger = true;
			this.transform.position = new Vector3(100f,100f,0);
			this.m_oExecutioner.NotifyExecuteEndFail(this);
		}
	}

	public int GetID(){
		return this.m_iID;
	}

	public void SetAllocator(Allocator alloc){
		this.m_oAllocatorOwner = alloc;
	}
	public Allocator GetAllocator(){
		return this.m_oAllocatorOwner;
	}
	
	public GameObject GetGameObject(){
		return this.gameObject;
	}

	[SerializeField] private Executioner 	m_oExecutioner;
	[SerializeField] private Allocator		m_oAllocatorOwner;
	//EXECUTABLE INTERFACE-------------------

	// Use this for initialization
	void Start () {
		this.m_bCollided = false;
		this.StopExecute();
		this.m_fSpeed = Constants.OBSTACLE_SPEED;
		this.m_fTimeToLife = Constants.OBSTACLE_ENEMY_TIME_TO_LIFE;
	}
	
	// Update is called once per frame
	void Update () {
		this.MoveEnemy();
	}

	private void MoveEnemy(){
		this.m_fLife += Time.deltaTime;
		if(this.m_fLife > this.m_fTimeToLife){
			this.StopExecute();
			this.NotifyExecutioner(false);
		}
		this.transform.Translate(Vector3.down*Time.deltaTime*this.m_fSpeed);
	}

	private void OnTriggerEnter(Collider other){
		if(!this.m_bCollided && other.tag.Equals("Projectile")){
			this.m_bCollided = true;
			this.StopExecute();
			this.NotifyExecutioner(false);
		}
	}

	private void OnCollisionEnter(Collision other){
		if(!this.m_bCollided && other.gameObject.tag.Equals("Player")){
			this.m_bCollided = true;
			this.StopExecute();
			this.NotifyExecutioner(true);
		}
	}

	[SerializeField] private Vector3 		m_vDirection;
	[SerializeField] private float 			m_fSpeed;
	[SerializeField] private float 			m_fTimeToLife;
	[SerializeField] private float 			m_fLife;
	[SerializeField] private bool			m_bCollided;
	[SerializeField] private int			m_iID;
}
