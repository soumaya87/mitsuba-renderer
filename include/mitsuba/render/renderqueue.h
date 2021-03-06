/*
    This file is part of Mitsuba, a physically based rendering system.

    Copyright (c) 2007-2011 by Wenzel Jakob and others.

    Mitsuba is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    Mitsuba is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#if !defined(__RENDERQUEUE_H)
#define __RENDERQUEUE_H

#include <mitsuba/mitsuba.h>
#include <mitsuba/render/rectwu.h>
#include <queue>

MTS_NAMESPACE_BEGIN

/** 
 * \brief Abstract render listener - can be used to react to 
 * progress messages (e.g. in a GUI)
 */
class MTS_EXPORT_RENDER RenderListener : public Object {
public:
	/// Called when work has begun in a rectangular image region
	virtual void workBeginEvent(const RenderJob *job, const RectangularWorkUnit *wu, int worker) = 0;
	
	/// Called when work has finished in a rectangular image region
	virtual void workEndEvent(const RenderJob *job, const ImageBlock *wr) = 0;
	
	/**
	 * \brief Called when the whole target image has been altered in some way.
	 * \param bitmap (Optional) When a bitmap representation of the image data
	 * 	   exists, this parameter can be used to pass it. Set to NULL by default.
	 */
	virtual void refreshEvent(const RenderJob *job, const Bitmap *bitmap = NULL) = 0;

	/// Called when a render job has completed successfully or unsuccessfully
	virtual void finishJobEvent(const RenderJob *job, bool cancelled) = 0;

	MTS_DECLARE_CLASS()
protected:
	virtual ~RenderListener() { }
};

/** 
 * Render queue - used to keep track of a number of scenes
 * that are simultaneously being rendered. Also distributes
 * events regarding these scenes to registered listeners.
 */
class MTS_EXPORT_RENDER RenderQueue : public Object {
public:
    enum EExecutionStrategy {
        ESerial, // start a new job only if no other is executing
        ETransparent // behaves just as usual
    };

public:
	/// Create a new render queue
	RenderQueue(EExecutionStrategy execStrategy = ETransparent);

	/// Return the current number of jobs in the queue
	inline size_t getJobCount() const { return m_jobs.size(); }

	/// Add a render job to the queue
	void addJob(RenderJob *thr);

	/// Remove a (finished) render job from the queue
	void removeJob(RenderJob *thr, bool wasCancelled);

	/// Register a render listener
	void registerListener(RenderListener *listener);

	/// Unregister a render listener
	void unregisterListener(RenderListener *listener);

	/** 
	 * Wait until the queue contains a certain number
	 * of scenes (or less).
	 */
	void waitLeft(size_t njobs) const;

	/// Releases resources held by recently finished jobs
	void join() const;

	/// Cause all render jobs to write out the current image
	void flush();

    /**
     *  Managed execution of a previously registered job. For
     *  now this leads only to serial execution of the single jobs.
     */
    void managedExecution(RenderJob *thr);

    /// change the managed execution strategy
    void setManagedExecutionStrategy(EExecutionStrategy es);

	/* Event distribution */
	void signalWorkBegin(const RenderJob *job, const RectangularWorkUnit *wu, int worker);
	void signalWorkEnd(const RenderJob *job, const ImageBlock *block);
	void signalFinishJob(const RenderJob *job, bool cancelled);
	void signalRefresh(const RenderJob *job, const Bitmap *bitmap);

	MTS_DECLARE_CLASS()
private:
	/// Virtual destructor
	virtual ~RenderQueue();
private:
	struct JobRecord {
		/* Only starting time for now */
		unsigned int startTime;
        /* The time the job has been waiting due to delayed execution */
        unsigned int waitTime;
        /* indicates if the job is or was delayed */
        bool delayed;

		inline JobRecord() { }
		inline JobRecord(unsigned int startTime)
			: startTime(startTime), waitTime(0), delayed(false) {
		}
	};

	std::map<RenderJob *, JobRecord> m_jobs;
	mutable std::vector<RenderJob *> m_joinList;
	mutable ref<Mutex> m_mutex, m_joinMutex;
	mutable ref<ConditionVariable> m_cond;
	ref<Timer> m_timer;
	std::vector<RenderListener *> m_listeners;
    std::queue<RenderJob *> m_waitingJobs;
    EExecutionStrategy m_managingStrategy;
};

MTS_NAMESPACE_END

#endif /* __RENDERQUEUE_H */
