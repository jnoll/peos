#include "text.h"

GtkWidget * action_detail( xmlNode *action) {
	char *name = NULL;
	xmlNode *cur = NULL;
	GtkWidget *text;
	xmlNode *child = NULL;
	int flag = FALSE;

	cur = action;

	text = gtk_text_new (NULL, NULL);
	gtk_widget_set_name (text, "text");
	gtk_widget_ref (text);
	gtk_object_set_data_full (GTK_OBJECT(Peos), "text", text,
				(GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (text);
	GTK_WIDGET_UNSET_FLAGS (text, GTK_CAN_FOCUS);

	if (strcmp(cur->name, "action") == 0)
	{
		//gtk_text_insert (GTK_TEXT (text), NULL, NULL, NULL, "ACTION : ", -1);
		name = xmlGetProp(action, "name");
		gtk_text_insert (GTK_TEXT (text), NULL, NULL, NULL, "\t\t\t\t\t\t\t\t\t\t\t", -1);
		gtk_text_insert (GTK_TEXT (text), NULL, NULL, NULL, name, -1);

		gtk_text_insert (GTK_TEXT (text), NULL, NULL, NULL,"\n\nState: ", -1);
		name = xmlGetProp(action, "state");
		gtk_text_insert (GTK_TEXT (text), NULL, NULL, NULL,name, -1);
		gtk_text_insert (GTK_TEXT (text), NULL, NULL, NULL,"\n", -1);

		for ( cur = action->children; cur; cur = cur->next)
		{
			if ( !xmlNodeIsText(cur) && !strcmp(cur->name, "script"))
			{

			      for ( child = action->children; child; child = child->next)
			      {

				if(!xmlNodeIsText(child) && !strcmp(child->name, "req_resource"))
				{

				gtk_text_insert (GTK_TEXT (text), NULL, NULL, NULL,"\nRequired Resources: ", -1);
				name = xmlGetProp(child, "name");
				gtk_text_insert (GTK_TEXT (text), NULL, NULL, NULL,name, -1);
			
				/*
				gtk_text_insert (GTK_TEXT (text), NULL, NULL, NULL," Value : ", -1);
				name = xmlGetProp(child, "value");
				gtk_text_insert (GTK_TEXT (text), NULL, NULL, NULL,name, -1);

				gtk_text_insert (GTK_TEXT (text), NULL, NULL, NULL," Qualifier: ", -1);
				name = xmlGetProp(child, "qualifier");
				gtk_text_insert (GTK_TEXT (text), NULL, NULL, NULL,name, -1);
				*/
                                
				gtk_text_insert (GTK_TEXT (text), NULL, NULL, NULL,"\n", -1);
				}

				if(!xmlNodeIsText(child) && !strcmp(child->name, "prov_resource"))
				{
				flag = TRUE;

				gtk_text_insert (GTK_TEXT (text), NULL, NULL, NULL,"\nProvided Resources: ", -1);
				name = xmlGetProp(child, "name");
				gtk_text_insert (GTK_TEXT (text), NULL, NULL, NULL,name, -1);

				/*

				gtk_text_insert (GTK_TEXT (text), NULL, NULL, NULL," Value : ", -1);
				name = xmlGetProp(child, "value");
				gtk_text_insert (GTK_TEXT (text), NULL, NULL, NULL,name, -1);

				gtk_text_insert (GTK_TEXT (text), NULL, NULL, NULL," Qualifier: ", -1);
				name = xmlGetProp(child, "qualifier");
				gtk_text_insert (GTK_TEXT (text), NULL, NULL, NULL,name, -1);
				*/

				gtk_text_insert (GTK_TEXT (text), NULL, NULL, NULL,"\n", -1);
				}
			}


			}
		}
		if(flag == FALSE)
			gtk_text_insert (GTK_TEXT (text), NULL, NULL, NULL,"\nNo Resources Provided \n", -1);


		gtk_text_insert (GTK_TEXT (text), NULL, NULL, NULL,"\nScript: ", -1);
		gtk_text_insert (GTK_TEXT (text), NULL, NULL, NULL,xmlNodeGetContent(action), -1);
	} else if ( strcmp(cur->name, "action") != 0) {
		gtk_text_insert (GTK_TEXT (text), NULL, NULL, NULL,"ACTION : ", -1);
		gtk_text_insert (GTK_TEXT (text), NULL, NULL, NULL,action->name, -1);
	}

  return text;
}




