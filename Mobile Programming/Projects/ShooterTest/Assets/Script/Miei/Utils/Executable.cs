using UnityEngine;

public interface Executable{
	void Execute();
	void StopExecute();
	bool IsExecuting();

	void SetExcutioner(Executioner exec);
	GameObject GetGameObject();
	void NotifyExecutioner(Executable executable,bool success);

	void SetPool(Pool<Executable> pool);
	Pool<Executable> GetPool();
}
