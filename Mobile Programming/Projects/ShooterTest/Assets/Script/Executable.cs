using UnityEngine;

public class Executable: MonoBehaviour{
	public virtual void Execute(){
		this.enabled = true;
	}

	public void StopExecute(){
		this.enabled = false;
	}

	public bool isExecuting(){
		return this.enabled;
	}
}
