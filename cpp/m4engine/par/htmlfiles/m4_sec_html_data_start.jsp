<!-- >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> m4_sec_html_data_start.html  -->

<%@ page import="java.util.Properties" %>
<%@ page import="com.meta4.session.*" %>
<%@ page import="com.meta4.configuration.*" %>
<%@ page import="com.meta4.redirect.*" %>

<%
response.setHeader("Cache-Control","no-cache"); //HTTP 1.1
response.setHeader("Pragma","no-cache"); //HTTP 1.0
response.setDateHeader ("Expires", 0); //prevents caching at the proxy server
%>

<%!
	
	private static String getStringValue(String sValue) {
		return ((sValue == null) || sValue.equals("") || sValue.equals("null")) ? null : sValue;
	}
	
	private static  int ExistsSession(HttpServletRequest ai_request){
		return (M4Context.getSession(ai_request) == null) ? 0 : 1;
}

	private static String getUserTempURI_50(HttpServletRequest ai_request){
		/*
		 * funcion que devuelve la uri del temp de un usuario (e.g. /temp/M4ADM_123456789)
		 * nunca terminada en BARRA
		 * tenemos el pathtempserverweb, el userpathtemserverweb y el thinclientroot
		 * thinclientroot debería de venir vacío, pero puede venir con algún valor.
		 * Lo que hacemos es entre estos tres valores devolver la uri.
		 */

		char c1= '/';
		char c2='\\';
		String sFileSeparator = "/";

		String stUserTempUri="";
		
		M4SessionManager zsessionmanager = M4Context.getSession(ai_request);
		String stPathUserTempServerWeb= (String) zsessionmanager.getPathTempServerWeb();
		stPathUserTempServerWeb=stPathUserTempServerWeb.replace(c2,c1);
		

		try{
			String stThinClientRoot=M4ConfigClient.getElement(M4VarConfigClient.M4_CFG_THINCLIENT_ROOT_TC);
			//String stThinClientRoot="";

			stThinClientRoot=stThinClientRoot.replace(c2,c1);

			if (stThinClientRoot.startsWith(sFileSeparator)){
				stThinClientRoot= stThinClientRoot.substring(1,stThinClientRoot.length());
			}
			if (stThinClientRoot.endsWith(sFileSeparator)){
				stThinClientRoot= stThinClientRoot.substring(0,stThinClientRoot.length()-1);
			}

			String stPathTempServerWeb=M4ConfigClient.getElement(M4VarConfigClient.M4_CFG_PATH_TEMP_SERVER_WEB);
			//String stPathTempServerWeb="c:\\inetpub\\wwwroot\\m4webservices\\temp\\";

			stPathTempServerWeb=stPathTempServerWeb.replace(c2,c1);


			if (stThinClientRoot==null || stThinClientRoot.equals("") || stThinClientRoot.equals("\\") || stThinClientRoot.equals("/"))
			{
				//está vacío el thinclientroot
				if (stPathUserTempServerWeb.lastIndexOf(sFileSeparator)==stPathUserTempServerWeb.length()-1){
					stPathUserTempServerWeb=stPathUserTempServerWeb.substring(0,stPathUserTempServerWeb.length()-1);
				}
				if (stPathTempServerWeb.lastIndexOf(sFileSeparator)==stPathTempServerWeb.length()-1){
					stPathTempServerWeb=stPathTempServerWeb.substring(0,stPathTempServerWeb.length()-1);
				}

				int iIndex=stPathTempServerWeb.lastIndexOf(sFileSeparator);
				if (iIndex>0){
					String st1=stPathTempServerWeb.substring(0,iIndex);
					String st2=stPathTempServerWeb.substring(iIndex,stPathTempServerWeb.length());

					String st3=stPathUserTempServerWeb.substring(stPathTempServerWeb.length(),stPathUserTempServerWeb.length());

					stUserTempUri= st2 + st3;
				}
			}else{
				//tenemos thinclientroot
				int iIndex=stPathUserTempServerWeb.lastIndexOf(stThinClientRoot);
				if (iIndex>0){
					stUserTempUri=sFileSeparator + stPathUserTempServerWeb.substring(iIndex, stPathUserTempServerWeb.length());
				}else{
					if (stPathUserTempServerWeb.lastIndexOf(sFileSeparator)==stPathUserTempServerWeb.length()){
						stPathUserTempServerWeb.substring(0,stPathUserTempServerWeb.length()-1);
					}

					iIndex=stPathTempServerWeb.lastIndexOf(sFileSeparator);
					if (iIndex>0){
						stUserTempUri=sFileSeparator + stThinClientRoot + stPathUserTempServerWeb.substring(iIndex,stPathUserTempServerWeb.length());
					}
				}
			}
			stUserTempUri=stUserTempUri.replace(c2,c1);
		}catch (Exception e){

		}

		//por si no está acabodo en un fileseparator.
		if (stUserTempUri.endsWith(sFileSeparator)){
			stUserTempUri=stUserTempUri.substring(0,stUserTempUri.length()-1);
		}

		return stUserTempUri;

	}

	public static String getRequestURI (HttpServletRequest ai_request) {
		
		String stRequestURI = "";

		stRequestURI  	     = ai_request.getRequestURI();

		return stRequestURI;
	}

%>

<!-- <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< m4_sec_html_data_start.html  -->
