function parse_UPPAAL(s,out_name)

fid=fopen(s);
raw_txt=textscan(fid,'%s');
fclose(fid);
txt=[];
for i=1:length(raw_txt{1,1})
    txt=[txt,sprintf(' %s',raw_txt{1,1}{i})];
end
sys_beg=strfind(txt,'system ');
temp=strfind(txt(sys_beg(1):end),';');
sys_end=temp(1)+sys_beg-2;
sys_def=txt(sys_beg+7:sys_end);
temp_name=strread(sys_def,'%s','delimiter',',');

edge_count=0;
for i=1:length(temp_name)-2
    temp_start_cand=strfind(txt,sprintf('%s</name>',temp_name{i}(2:end)));
    temp_start=temp_start_cand(1);
    
    temp_end_cand=strfind(txt(temp_start:end),'</template>');
    temp_end=temp_start+temp_end_cand(1);
    temp_cont=txt(temp_start:temp_end);
    
    tran_starts=strfind(temp_cont,'<transition>');
    tran_ends=strfind(temp_cont,'</transition>');
    
    tran_insert=[];
    for j=1:length(tran_ends)
        tran_cont=temp_cont(tran_starts(j):tran_ends(j));
        assign_label=strfind(tran_cont,'<label kind="assignment"');
        
        if isempty(assign_label)
            label_ends=strfind(tran_cont,'</label>');
            tran_insert{j,1}=label_ends(end)+8+tran_starts(j)+temp_start-2;
            tran_insert{j,2}=sprintf('<label kind="assignment" x="-100" y="-100">t%d=1</label>',edge_count);
            
        else
            label_end=strfind(tran_cont(assign_label:end),'</label>');
            tran_insert{j,1}=assign_label+label_end(1)+tran_starts(j)+temp_start-3;
            tran_insert{j,2}=sprintf(',t%d=1',edge_count);
        end
        edge_count=edge_count+1;
    end

    tran_insert=[{1},{[]};tran_insert];
    newtxt=[];
    for j=1:size(tran_insert,1)-1
        newtxt=[newtxt,tran_insert{j,2},txt(tran_insert{j,1}:tran_insert{j+1,1}-1)];
    end
    newtxt=[newtxt,tran_insert{end,2},txt(tran_insert{end,1}:end)];
    txt=newtxt;
end

dec=strfind(txt,'<declaration>');
var_beg=dec(1)+13;
var_insert=[];
query_insert='E<> (';
for i=0:edge_count-1
   var_insert=[var_insert,sprintf('bool t%d=0;\n',i)]; 
   query_insert=[query_insert,sprintf('t%d==1 && ',i)];
end
query_insert=[query_insert(1:end-3),')'];
fid=fopen(sprintf('%s.q',out_name),'w');
for i=1:length(query_insert)
        if query_insert(i)==' '
            fprintf(fid,' ');
        else
            fprintf(fid,query_insert(i));
        end
end
    fclose(fid);
    
txt=[txt(1:var_beg-1),var_insert,txt(var_beg:end)];
  fid=fopen(sprintf('%s.xml',out_name),'w');
    for i=2:length(txt)
        if txt(i)==' '
            fprintf(fid,' ');
        else
            fprintf(fid,txt(i));
        end
    end
    fclose(fid);