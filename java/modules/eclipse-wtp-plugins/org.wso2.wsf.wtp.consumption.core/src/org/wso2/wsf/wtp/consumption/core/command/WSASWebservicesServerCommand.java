/*
 * Copyright 2005,2006 WSO2, Inc. http://www.wso2.org
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.wso2.wsf.wtp.consumption.core.command;

import org.eclipse.core.commands.ExecutionException;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.IAdaptable;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.Status;
import org.eclipse.wst.common.environment.IEnvironment;
import org.eclipse.wst.common.environment.IStatusHandler;
import org.eclipse.wst.common.frameworks.datamodel.AbstractDataModelOperation;
import org.wso2.wsf.wtp.consumption.core.data.DataModel;
import org.wso2.wsf.wtp.consumption.core.messages.WSASConsumptionUIMessages;
import org.wso2.wsf.wtp.consumption.core.utils.ContentCopyUtils;
import org.wso2.wsf.wtp.consumption.core.utils.FileUtils;

public class WSASWebservicesServerCommand extends
AbstractDataModelOperation {

	String project;
	DataModel model;
	public WSASWebservicesServerCommand(DataModel model, String project){
		this.model=model;
		this.project = project;
	}

	public IStatus execute(IProgressMonitor monitor, IAdaptable info)
	throws ExecutionException {
		//Copy the axis2 libs in to this client project
		IStatus status = Status.OK_STATUS;
		IEnvironment environment = getEnvironment();
		IStatusHandler statusHandler = environment.getStatusHandler();	
		
		String web_infLib[] ={WSASConsumptionUIMessages.DIR_WEB_INF,WSASConsumptionUIMessages.DIR_LIB};


		String workspaceDirectory = ResourcesPlugin.getWorkspace().getRoot()
									.getLocation().toOSString();
		String currentDynamicWebProjectDir = FileUtils.addAnotherNodeToPath(workspaceDirectory, 
																			project); 
		model.setWebProjectName(project);
		
		// Location of the axis2 plugin to copy tho the current project
		String correctUserDir = Platform.getInstallLocation().getURL().getPath().toString();
		
		//TODO The Web content directory can be different. cater that also
		String webContainerDirString = FileUtils.addAnotherNodeToPath(currentDynamicWebProjectDir, 
				WSASConsumptionUIMessages.DIR_WEBCONTENT);
//		String webContainerLibDirString = FileUtils.addAnotherNodeToPath(webContainerDirString, 
//														WSASConsumptionUIMessages.WEB_INF_LIB);
		String webContainerLibDirString = FileUtils.addNodesToPath(webContainerDirString, web_infLib);
		
//		String axis2LibLocation = FileUtils.addAnotherNodeToPath(correctUserDir,
//											WSASConsumptionUIMessages.PATH_TO_AXIS2_LIBS);
		
		
		String axis2plugin[] = {WSASConsumptionUIMessages.AXIS2_PLUGIN,WSASConsumptionUIMessages.DIR_AXIS2};
		String axis2ProjectDir = FileUtils.addNodesToPath(workspaceDirectory, axis2plugin);
		String axis2LibLocation =  FileUtils.addNodesToPath(axis2ProjectDir, web_infLib);
		
		

		
		ContentCopyUtils contentCopyUtils = new ContentCopyUtils();
		status = contentCopyUtils.copyDirectoryRecursivelyIntoWorkspace(axis2LibLocation, 
																		webContainerLibDirString, 
																		monitor, 
																		statusHandler);
		return status;
	}

}
