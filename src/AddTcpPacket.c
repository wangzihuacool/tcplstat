/*
 * tcplstat - TCP packets monitor and statistical tool
 * author	: calvin
 * email	: calvinwilliams@163.com
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory.
 */

#include "tcplstat_in.h"

/* ��TCP������Ч�غ��г�������SQL��� */
static char *FindSql( char *packet_data_intercepted , UINT32 packet_data_len_intercepted , int *p_sql_len )
{
	char		*p1 = NULL ;
	char		*p2 = NULL ;
	char		*end = packet_data_intercepted + packet_data_len_intercepted - 1 ;

    p1 = memistr2_region( packet_data_intercepted , "EXPLAIN" , end , 1 ) ;
    if( p1 )
    {
        (*p_sql_len) = LengthUtilEndOfText( p1+7 , end ) ;
        (*p_sql_len) += 7 ;
        return p1;
    }
	
	p1 = memistr2_region( packet_data_intercepted , "SELECT" , end , 1 ) ;
	if( p1 )
	{
		/* Ϊ��ץ��"select function();" */
		/*
		p2 = memistr2_region( p1+6 , "FROM" , end , 0 ) ;
		if( p2 )
		{
			(*p_sql_len) = LengthUtilEndOfText( p2+4 , end ) ;
			(*p_sql_len) += p2-p1 + 4 ;
			return p1;
		}
		*/
		(*p_sql_len) = LengthUtilEndOfText( p1+6 , end ) ;
		(*p_sql_len) += 6 ;
		return p1;
	}
	
	p1 = memistr2_region( packet_data_intercepted , "UPDATE" , end , 1 ) ;
	if( p1 )
	{
		p2 = memistr2_region( p1+6 , "SET" , end , 0 ) ;
		if( p2 )
		{
			(*p_sql_len) = LengthUtilEndOfText( p2+3 , end ) ;
			(*p_sql_len) += p2-p1 + 3 ;
			return p1;
		}
	}
	
	p1 = memistr2_region( packet_data_intercepted , "INSERT" , end , 1 ) ;
	if( p1 )
	{
		p2 = memistr2_region( p1+7 , "INTO" , end , 0 ) ;
		if( p2 )
		{
			(*p_sql_len) = LengthUtilEndOfText( p2+4 , end ) ;
			(*p_sql_len) += p2-p1 + 4 ;
			return p1;
		}
	}
	
	p1 = memistr2_region( packet_data_intercepted , "DELETE" , end , 1 ) ;
	if( p1 )
	{
		p2 = memistr2_region( p1+6 , "FROM" , end , 0 ) ;
		if( p2 )
		{
			(*p_sql_len) = LengthUtilEndOfText( p2+4 , end ) ;
			(*p_sql_len) += p2-p1 + 4 ;
			return p1;
		}
	}
	
	p1 = memistr2_region( packet_data_intercepted , "CREATE" , end , 1 ) ;
	if( p1 )
	{
		(*p_sql_len) = LengthUtilEndOfText( p1+6 , end ) ;
		(*p_sql_len) += 6 ;
		return p1;
	}
	
	p1 = memistr2_region( packet_data_intercepted , "DROP" , end , 1 ) ;
	if( p1 )
	{
		(*p_sql_len) = LengthUtilEndOfText( p1+4 , end ) ;
		(*p_sql_len) += 4 ;
		return p1;
	}
	
	p1 = memistr2_region( packet_data_intercepted , "ALTER" , end , 1 ) ;
	if( p1 )
	{
		(*p_sql_len) = LengthUtilEndOfText( p1+5 , end ) ;
		(*p_sql_len) += 5 ;
		return p1;
	}
	
	p1 = memistr2_region( packet_data_intercepted , "BEGIN" , end , 1 ) ;
	if( p1 )
	{
		(*p_sql_len) = LengthUtilEndOfText( p1+5 , end ) ;
		(*p_sql_len) += 6 ;
		return p1;
	}
	
	p1 = memistr2_region( packet_data_intercepted , "COMMIT" , end , 1 ) ;
	if( p1 )
	{
		(*p_sql_len) = LengthUtilEndOfText( p1+6 , end ) ;
		(*p_sql_len) += 6 ;
		return p1;
	}
	
	p1 = memistr2_region( packet_data_intercepted , "ROLLBACK" , end , 1 ) ;
	if( p1 )
	{
		(*p_sql_len) = LengthUtilEndOfText( p1+8 , end ) ;
		(*p_sql_len) += 8 ;
		return p1;
	}
	
	p1 = memistr2_region( packet_data_intercepted , "GRANT" , end , 1 ) ;
	if( p1 )
	{
		(*p_sql_len) = LengthUtilEndOfText( p1+5 , end ) ;
		(*p_sql_len) += 5 ;
		return p1;
	}
	
	p1 = memistr2_region( packet_data_intercepted , "TRUNCATE" , end , 1 ) ;
	if( p1 )
	{
		(*p_sql_len) = LengthUtilEndOfText( p1+8 , end ) ;
		(*p_sql_len) += 8 ;
		return p1;
	}

    p1 = memistr2_region( packet_data_intercepted , "SET" , end , 1 ) ;
    if( p1 )
    {
        (*p_sql_len) = LengthUtilEndOfText( p1+3 , end ) ;
        (*p_sql_len) += 3 ;
        return p1;
    }

    p1 = memistr2_region( packet_data_intercepted , "SHOW" , end , 1 ) ;
    if( p1 )
    {
        (*p_sql_len) = LengthUtilEndOfText( p1+4 , end ) ;
        (*p_sql_len) += 4 ;
        return p1;
    }

    p1 = memistr2_region( packet_data_intercepted , "RENAME" , end , 1 ) ;
    if( p1 )
    {
        (*p_sql_len) = LengthUtilEndOfText( p1+6 , end ) ;
        (*p_sql_len) += 6 ;
        return p1;
    }

    p1 = memistr2_region( packet_data_intercepted , "START" , end , 1 ) ;
    if( p1 )
    {
        (*p_sql_len) = LengthUtilEndOfText( p1+5 , end ) ;
        (*p_sql_len) += 5 ;
        return p1;
    }

	return 0;
}

/* ��TCP������Ч�غ��г�������HTTP���� */
static char *FindHttp( char *packet_data_intercepted , UINT32 packet_data_len_intercepted , int *p_http_first_line_len )
{
	char		*p1 = NULL ;
	char		*p2 = NULL ;
	char		*end = packet_data_intercepted + packet_data_len_intercepted - 1 ;
	
	if( packet_data_len_intercepted > 3 && ( MEMCMP( packet_data_intercepted , == , "GET" , 3 ) ) )
	{
		p1 = memistr2_region( packet_data_intercepted+3 , "HTTP" , end , 0 ) ;
		if( p1 )
		{
			p2 = strpbrk( p1+4 , "\r\n" ) ;
			(*p_http_first_line_len) = p2 - packet_data_intercepted ;
			return packet_data_intercepted;
		}
	}
	
	if( packet_data_len_intercepted > 7 && ( MEMCMP( packet_data_intercepted , == , "OPTIONS" , 7 ) ) )
	{
		p1 = memistr2_region( packet_data_intercepted+7 , "HTTP" , end , 0 ) ;
		if( p1 )
		{
			p2 = strpbrk( p1+4 , "\r\n" ) ;
			(*p_http_first_line_len) = p2 - packet_data_intercepted ;
			return packet_data_intercepted;
		}
	}
	
	if( packet_data_len_intercepted > 4 && ( MEMCMP( packet_data_intercepted , == , "HEAD" , 4 ) ) )
	{
		p1 = memistr2_region( packet_data_intercepted+4 , "HTTP" , end , 0 ) ;
		if( p1 )
		{
			p2 = strpbrk( p1+4 , "\r\n" ) ;
			(*p_http_first_line_len) = p2 - packet_data_intercepted ;
			return packet_data_intercepted;
		}
	}
	
	if( packet_data_len_intercepted > 4 && ( MEMCMP( packet_data_intercepted , == , "POST" , 4 ) ) )
	{
		p1 = memistr2_region( packet_data_intercepted+4 , "HTTP" , end , 0 ) ;
		if( p1 )
		{
			p2 = strpbrk( p1+4 , "\r\n" ) ;
			(*p_http_first_line_len) = p2 - packet_data_intercepted ;
			return packet_data_intercepted;
		}
	}
	
	if( packet_data_len_intercepted > 3 && ( MEMCMP( packet_data_intercepted , == , "PUT" , 3 ) ) )
	{
		p1 = memistr2_region( packet_data_intercepted+3 , "HTTP" , end , 0 ) ;
		if( p1 )
		{
			p2 = strpbrk( p1+4 , "\r\n" ) ;
			(*p_http_first_line_len) = p2 - packet_data_intercepted ;
			return packet_data_intercepted;
		}
	}
	
	if( packet_data_len_intercepted > 6 && ( MEMCMP( packet_data_intercepted , == , "DELETE" , 6 ) ) )
	{
		p1 = memistr2_region( packet_data_intercepted+6 , "HTTP" , end , 0 ) ;
		if( p1 )
		{
			p2 = strpbrk( p1+4 , "\r\n" ) ;
			(*p_http_first_line_len) = p2 - packet_data_intercepted ;
			return packet_data_intercepted;
		}
	}
	
	if( packet_data_len_intercepted > 5 && ( MEMCMP( packet_data_intercepted , == , "TRACE" , 5 ) ) )
	{
		p1 = memistr2_region( packet_data_intercepted+5 , "HTTP" , end , 0 ) ;
		if( p1 )
		{
			p2 = strpbrk( p1+4 , "\r\n" ) ;
			(*p_http_first_line_len) = p2 - packet_data_intercepted ;
			return packet_data_intercepted;
		}
	}
	
	if( packet_data_len_intercepted > 7 && ( MEMCMP( packet_data_intercepted , == , "CONNECT" , 7 ) ) )
	{
		p1 = memistr2_region( packet_data_intercepted+7 , "HTTP" , end , 0 ) ;
		if( p1 )
		{
			p2 = strpbrk( p1+4 , "\r\n" ) ;
			(*p_http_first_line_len) = p2 - packet_data_intercepted ;
			return packet_data_intercepted;
		}
	}
	
	return 0;
}

/* ����TCP���鵽��ϸ������ */
int AddTcpPacket( struct TcplStatEnv *p_env , struct TcplSession *p_tcpl_session , unsigned char direction_flag , struct NetinetTcpHeader *tcphdr , char *packet_data_intercepted , UINT32 packet_data_len_intercepted , UINT32 packet_data_len_actually )
{
	struct TcplPacket	*p_tcpl_packet = NULL ;
	struct TcplPacket	*p_last_tcpl_packet = NULL ;
	struct TcplPacket	*p_last_oppo_tcpl_packet = NULL ;
	
	if( p_env->unused_tcpl_packet_count == 0 )
	{
		p_tcpl_packet = (struct TcplPacket *)malloc( sizeof(struct TcplPacket) ) ;
		if( p_tcpl_packet == NULL )
		{
			fprintf( p_env->fp , "*** ERROR : alloc failed , errno[%d]\n" , errno );
			exit(1);
		}
		memset( p_tcpl_packet , 0x00 , sizeof(struct TcplPacket) );
	}
	else
	{
		REUSE_TCPL_PACKET( p_env , p_tcpl_packet )
	}
	
	COPY_TIMEVAL( p_tcpl_packet->timestamp , p_env->fixed_timestamp )
	
	/* ͳ������һ��TCP������ӳ� */
	if( ! list_empty( & (p_tcpl_session->tcpl_packets_trace_list.this_node) ) )
	{
		p_last_tcpl_packet = list_last_entry( & (p_tcpl_session->tcpl_packets_trace_list.this_node) , struct TcplPacket , this_node ) ;
		COPY_TIMEVAL( p_tcpl_packet->last_packet_elapse , p_tcpl_packet->timestamp );
		DIFF_TIMEVAL( p_tcpl_packet->last_packet_elapse , p_last_tcpl_packet->timestamp )
	}
	
	/* ͳ������һ������TCP������ӳ� */
	if( direction_flag == TCPLPACKET_DIRECTION )
		p_last_oppo_tcpl_packet = p_tcpl_session->p_recent_oppo_packet ;
	else
		p_last_oppo_tcpl_packet = p_tcpl_session->p_recent_packet ;
	if( p_last_oppo_tcpl_packet )
	{
		COPY_TIMEVAL( p_tcpl_packet->last_oppo_packet_elapse , p_tcpl_packet->timestamp );
		DIFF_TIMEVAL( p_tcpl_packet->last_oppo_packet_elapse , p_last_oppo_tcpl_packet->timestamp )
	}
	
	/* ���TCP������ϸ�ṹ */
	p_tcpl_packet->direction_flag = direction_flag ;
	sprintf( p_tcpl_packet->packet_flags , "%c%c%c%c%c%c" , TH_FLAG(tcphdr,TH_SYN)?'S':'.' , TH_FLAG(tcphdr,TH_FIN)?'F':'.' , TH_FLAG(tcphdr,TH_PSH)?'P':'.' , TH_FLAG(tcphdr,TH_ACK)?'A':'.' , TH_FLAG(tcphdr,TH_RST)?'R':'.' , TH_FLAG(tcphdr,TH_URG)?'U':'.' );
	
	p_tcpl_packet->packet_data_len_intercepted = packet_data_len_intercepted ;
	p_tcpl_packet->packet_data_len_actually = packet_data_len_actually ;
	if( packet_data_len_actually > 0 )
		p_tcpl_packet->packet_data_intercepted = memndup( packet_data_intercepted , packet_data_len_intercepted ) ;
	if( p_tcpl_packet == NULL )
	{
		fprintf( p_env->fp , "*** ERROR : alloc tcpl_packet failed , errno[%d]\n" , errno );
		free( p_tcpl_packet );
		return -1;
	}
	
	/* TCP������ϸ�ҽӵ������� */
	list_add_tail( & (p_tcpl_packet->this_node) , & (p_tcpl_session->tcpl_packets_trace_list.this_node) );
	
	/* ��̽SQL��䣬���´���Ч����ʱͳ�ƺ�ʱ����� */
	if( p_env->cmd_line_para.output_sql && packet_data_len_intercepted > 0 )
	{
		if( direction_flag == TCPLPACKET_DIRECTION )
		{
			if( p_tcpl_session->sql )
			{
				p_tcpl_session->sql = NULL ;
			}
			
			p_tcpl_session->sql = FindSql( p_tcpl_packet->packet_data_intercepted , p_tcpl_packet->packet_data_len_intercepted , & (p_tcpl_session->sql_len) ) ;
			if( p_tcpl_session->sql )
			{
				ReplaceCharInString( p_tcpl_session->sql , p_tcpl_session->sql_len , '\n' , ' ' );
				
				if( p_env->cmd_line_para.output_debug )
				{
					fprintf( p_env->fp , "q | %s.%06ld | %.*s\n"
						, ConvDateTimeHumanReadable(p_tcpl_packet->timestamp.tv_sec) , p_tcpl_packet->timestamp.tv_usec
						, p_tcpl_session->sql_len , p_tcpl_session->sql );
				}
			}
		}
		else if( direction_flag == TCPLPACKET_OPPO_DIRECTION )
		{
			if( p_tcpl_session->sql && p_last_oppo_tcpl_packet )
			{
				fprintf( p_env->fp , "Q | %s.%06ld | %ld.%06ld | %.*s\n"
					, ConvDateTimeHumanReadable(p_tcpl_packet->timestamp.tv_sec) , p_tcpl_packet->timestamp.tv_usec
					, p_tcpl_packet->last_oppo_packet_elapse.tv_sec , p_tcpl_packet->last_oppo_packet_elapse.tv_usec
					, p_tcpl_session->sql_len , p_tcpl_session->sql );
				p_tcpl_session->sql = NULL ;
			}
		}
	}
	
	/* ��̽HTTP���У����´���Ч����ʱͳ�ƺ�ʱ����� */
	if( p_env->cmd_line_para.output_http && p_tcpl_packet->packet_data_len_intercepted > 0 )
	{
		if( p_tcpl_session->http_first_line && p_last_oppo_tcpl_packet )
		{
			fprintf( p_env->fp , "H | %s.%06ld | %ld.%06ld | %.*s\n"
				, ConvDateTimeHumanReadable(p_tcpl_packet->timestamp.tv_sec) , p_tcpl_packet->timestamp.tv_usec
				, p_tcpl_packet->last_oppo_packet_elapse.tv_sec , p_tcpl_packet->last_oppo_packet_elapse.tv_usec
				, p_tcpl_session->http_first_line_len , p_tcpl_session->http_first_line );
			p_tcpl_session->http_first_line = NULL ;
		}
		
		p_tcpl_session->http_first_line = FindHttp( p_tcpl_packet->packet_data_intercepted , p_tcpl_packet->packet_data_len_intercepted , & (p_tcpl_session->http_first_line_len) ) ;
		if( p_tcpl_session->http_first_line )
		{
			if( p_env->cmd_line_para.output_debug )
			{
				fprintf( p_env->fp , "h | %s.%06ld | %.*s\n"
					, ConvDateTimeHumanReadable(p_tcpl_packet->timestamp.tv_sec) , p_tcpl_packet->timestamp.tv_usec
					, p_tcpl_session->http_first_line_len , p_tcpl_session->http_first_line );
			}
		}
	}
	
	/* ����������ֺͷ��֣�ͳ������/�������С��ƽ���͡�����ӳ� */
	if( direction_flag == TCPLPACKET_DIRECTION )
	{
		p_tcpl_session->p_recent_packet = p_tcpl_packet ;
	}
	else
	{
		p_tcpl_session->p_recent_oppo_packet = p_tcpl_packet ;
	}
	
	if( p_tcpl_session->state == TCPLSESSION_STATE_CONNECTED )
	{
		ADD_TIMEVAL( p_tcpl_session->total_packet_elapse_for_avg , p_tcpl_packet->last_packet_elapse )
		
		if( p_tcpl_session->min_packet_flag == 0 || COMPARE_TIMEVAL( p_tcpl_packet->last_packet_elapse , < , p_tcpl_session->min_packet_elapse ) )
		{
			p_tcpl_session->min_packet_flag = 1 ;
			COPY_TIMEVAL( p_tcpl_session->min_packet_elapse , p_tcpl_packet->last_packet_elapse )
		}
		if( p_tcpl_session->max_packet_flag == 0 || COMPARE_TIMEVAL( p_tcpl_packet->last_packet_elapse , > , p_tcpl_session->max_packet_elapse ) )
		{
			p_tcpl_session->max_packet_flag = 1 ;
			COPY_TIMEVAL( p_tcpl_session->max_packet_elapse , p_tcpl_packet->last_packet_elapse )
		}
		
		ADD_TIMEVAL( p_tcpl_session->total_oppo_packet_elapse_for_avg , p_tcpl_packet->last_oppo_packet_elapse )
		
		if( p_tcpl_session->min_oppo_packet_flag == 0 || COMPARE_TIMEVAL( p_tcpl_packet->last_oppo_packet_elapse , < , p_tcpl_session->min_oppo_packet_elapse ) )
		{
			p_tcpl_session->min_oppo_packet_flag = 1 ;
			COPY_TIMEVAL( p_tcpl_session->min_oppo_packet_elapse , p_tcpl_packet->last_oppo_packet_elapse )
		}
		if( p_tcpl_session->max_oppo_packet_flag == 0 || COMPARE_TIMEVAL( p_tcpl_packet->last_oppo_packet_elapse , > , p_tcpl_session->max_oppo_packet_elapse ) )
		{
			p_tcpl_session->max_oppo_packet_flag = 1 ;
			COPY_TIMEVAL( p_tcpl_session->max_oppo_packet_elapse , p_tcpl_packet->last_oppo_packet_elapse )
		}
		
		p_tcpl_session->total_packet_trace_count++;
		p_tcpl_session->total_packet_trace_data_len += packet_data_len_actually ;
	}
	
	if( p_env->cmd_line_para.output_debug )
		OUTPUT_PACKET_EVENT( p_tcpl_session , p_tcpl_packet )
	
	return 0;
}

