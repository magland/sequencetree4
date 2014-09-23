/****************************************************************************
 This file is part of SequenceTree
 Copyright (C) 2005-2008 Jeremy Magland (Jeremy.Magland@gmail.com)

 SequenceTree is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 SequenceTree is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with SequenceTree; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*****************************************************************************/


#ifndef stmetasequencesimulator_H
#define stmetasequencesimulator_H

#include "stmetasequence.h"
#include "stsimscannerblocklist.h"
#include <QProcess>
#include <QObject>

class STMetaSequenceSimulator : public QObject {
	Q_OBJECT
public:
	STMetaSequenceSimulator();
	virtual ~STMetaSequenceSimulator();
	void setMetaSequence(STMetaSequence *Seq);
	STMetaSequence *metaSequence() {return m_sequence;}
	STSimScannerBlock *retrieveBlock(long block_index);
	bool isCompiled();
	void compile(bool wait_for_finished=false); //initiates compilation, see compilationFinished
	void setCompiled(bool val) {m_is_compiled=val;}
	void computeStatistics(bool wait_for_finished=false); //initiates computation of statistics, see statisticsFinished
	bool runEntireSequence(bool wait_for_finished);
	double totalDuration();
	long numBlocks();
	double totalSAR();
	void clearBlocks() {m_blocklist.clear();}
	double maxRFAmplitude() {return m_max_RF_amplitude;}
	long maxLoadedBlockIndex() {return m_first_block_index+m_blocklist.count();}
	QString binDirectory() {return m_bin_directory;}
	QString tmpDirectory() {return m_tmp_directory;}
	QString srcDirectory() {return m_src_directory;}
	void setDirectories(QString bin_dir,QString tmp_dir,QString src_dir);
	void minimizeParameter(STMetaNode *N,QString param_name);
	void maximizeParameter(STMetaNode *N,QString param_name);
	void writeRawTemplateFile(QString fname);
	QString executableFileName() {return m_executable_fname;}
private slots:
	void slot_compile_message();
	void slot_compile_finished();
	
	void slot_simulate_message();
	void slot_simulate_finished();
signals:
	void compilationStarted();
	void compilationMessage(QString msg);
	void compilationFinished();
	
	void simulationStarted();
	void simulationMessage(QString msg);
	void runFinished(bool result);
	void statFinished(bool result);
private:
	STMetaSequence *m_sequence;
	double m_total_duration;
	long m_num_blocks;
	double m_total_SAR;
	bool m_is_compiled;
	long m_hold_start_block_index;
	STSimScannerBlockList m_blocklist;
	long m_first_block_index;
	QProcess *m_compile_process;
	QProcess *m_simulate_process;
	QString m_executable_fname;
	QString m_parameter_output_file;
	QString m_simulation_output_file;
	QString m_results_output_file;
	double m_max_RF_amplitude;
	QString m_runmode;
	
	QString m_bin_directory;
	QString m_tmp_directory;
	QString m_src_directory;
	
	bool run_sequence(long block_min,long block_max,QString runmode,QString param1="",bool wait_for_finished=false);
	bool check_sequence();
	void load_statistics_from_file(QString fname);
	bool determine_success_from_file(QString results_fname);
	QString create_new_executable_fname();
	void cleanup_old_executables();
};

#endif
